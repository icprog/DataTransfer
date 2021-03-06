﻿#include "CollectorBase.h"
#include "pathbuilder.h"
#include "ctkPublicFun.h"
#include "curlftp.h"
#include "DataBase.h"
#include <QUrl>
#include <QRegExp>
#include <QFileInfoList>
#include <QStringList>
#include <QMutexLocker>
#include <qthread.h>
#include <lastCollectTime.h>
#include <algorithm>
#include <QtConcurrent/QtConcurrent>
#include <QSharedMemory>
#include <QByteArray>
#include <QDataStream>
//#include <QHostAddress>

#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

// 初始化静态成员（自定义类对象，该类必须有默认构造函数）
TransCollectTimeList CollectorBase::m_lstTCtime;
bool CollectorBase::m_bReadTime = false;
QMutex CollectorBase::m_mutexTime;
//QString CollectorBase::m_strTctimePath;
QString CollectorBase::m_strSttimePath;

CollectorBase::CollectorBase(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize, QObject *pParent)
    : m_oCond(in_oCond),
      m_oLocker(in_oLocker),
      m_iLogsize(in_iLogsize),
      //m_oRcfClient(RCF::TcpEndpoint(50001)),
      QObject(pParent)		//从QObject继承的类需要写pParent!!
{
    m_bFinish = true;

    m_pLastTime = NULL;
    m_bRun = false;
    m_nLineState = 0;
	m_bBeingDeleted = false;

    connect(this, SIGNAL(begin()), this, SLOT(onBegined()));

    m_oId = QUuid::createUuid();

}

CollectorBase::~CollectorBase()
{
    emit showIdentify(m_oId.toString());
}

void CollectorBase::init()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void CollectorBase::unit()
{
    curl_global_cleanup();
}

bool CollectorBase::filterFileName(const QFileInfo &fi, const QString &strFilePath, FileInfo &fileInfo)
{
    return true;

}

bool CollectorBase::filterFileName(FileInfo &fileInfo)
{
    QRegExp reg;
    reg.setPattern(m_collectSet.fileTemplate);
    if (reg.exactMatch(QString::fromStdString(fileInfo.strFileName)))
    {
        return true;
    }

    return false;
}

bool CollectorBase::filterFileName(const QString &fileName)
{
    QRegExp reg;
    reg.setPattern(m_collectSet.fileTemplate);
    if (reg.exactMatch(fileName))
    {
        return true;
    }

    return false;
}

bool CollectorBase::IsNewFile(FileInfo &fileInfo)
{
    return true;
}

bool CollectorBase::ReadLastTime()
{
    return true;
}


void CollectorBase::showMsg(const QString &str)
{
    emit print(QString("[%1]: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(str));
}

void CollectorBase::emitLog(const QString &info, int infoType)
{
    //m_oLocker.lock();

    //if (m_iLogsize > 100)
    //{
    //	m_oCond.wait(&m_oLocker);
    //}
    //m_oLocker.unlock();
    //m_iLogsize++;
    //emit showLog(m_collectSet, info, infoType);

    //QSLOG_DEBUG("LOG");
    try
    {
        //printf("111111111111111111\n");
        //m_oRcfClient->print(m_collectSet.dirName.toLocal8Bit().toStdString(),
        //                    m_collectSet.dirID.toLocal8Bit().toStdString(),
        //                    info.toLocal8Bit().toStdString(), infoType);
        //QByteArray datagram;
        //QDataStream stream(&datagram, QIODevice::WriteOnly);
        //quint16 iMsgLen = 0;
        //QString msgType = "Collect";
        //stream << iMsgLen << msgType << m_collectSet.dirName << m_collectSet.dirID << info << infoType;
        //stream.device()->seek(0);
        //iMsgLen = datagram.length() - sizeof(quint16);
        //stream << iMsgLen;
        //qint64 iLen = m_oLogSocket.writeDatagram(datagram, QHostAddress::LocalHost, m_iUdpLogPort);

        m_oLogSender.sendCollectMsg(m_collectSet.dirName, m_collectSet.dirID, info, infoType);

    }
    catch (std::exception &ex)
    {
        QSLOG_ERROR(QString("rcf exception:").arg(ex.what()));
    }



    // 通过发送消息的方式通知控制进程
    // QThread::msleep(20);
}


void CollectorBase::emitBroadCast(const QString &srcfile, const QString &dstfile)
{
    m_oLogSender.sendBroadCastMsg(m_collectSet.dirName, srcfile, dstfile);
}


QString CollectorBase::getDestFilePath(const QString &srcFileFullPath, const QString &fileName, const CollectUser &cUser, const QDateTime &dt, bool enableTimeRule)
{
    QString destPath = cUser.user.rootPath + cUser.rltvPath;
    if (destPath.right(1) != "/")
    {
        destPath += "/";
    }
    else if (destPath == "//")
    {
        destPath = "/";
    }
	if (!cUser.bKeepDir)	// 不保持原目录结构
    {
		
        //return destPath;
    }
    else			// 保持原目录结构
    {
        // 从原始路径提取相对路径
        int srcRlvtPathLen = m_collectSet.rltvPath.length();
        if (m_collectSet.rltvPath.right(1) != "/")
        {
            srcRlvtPathLen += 1;
        }
        int subLen = srcFileFullPath.length() - srcRlvtPathLen - fileName.length() - 1;
        QString subPath("");
        if (subLen > 0)
        {
            subPath = srcFileFullPath.mid(srcRlvtPathLen, subLen);
        }
        destPath += subPath;
    }

    if (enableTimeRule)
    {
        destPath = CPathBuilder::getFinalPathFromUrl(destPath, dt).at(0);
    }

    // 根据配置的规则，替换目录的目标名称
    destPath = CPathBuilder::getFinalPathFromUrlV2(destPath, fileName);

    if (destPath.right(1) != "/")
    {
        destPath += "/";
    }
    //QSLOG_DEBUG(destPath);
    return destPath;
}

void CollectorBase::doWork()
{
    if (m_bFinish)
    {
        emit begin();
    }
}

void CollectorBase::onBegined()
{
    // 将该函数丢到子线程中执行，防止阻塞主线程
    QtConcurrent::run(QThreadPool::globalInstance(), this, &CollectorBase::getNewFiles);

    //getNewFiles();
}

bool CollectorBase::testFileConnection(QString strUrl)
{
    if (strUrl.size() < 2)
    {
        return false;
    }

    QUrl url = QUrl::fromLocalFile(strUrl);
    if (url.isLocalFile())
    {
        QDir qdir(strUrl);
        if (qdir.exists())
        {
            return true;
        }
    }

    return false;
}

bool CollectorBase::testFtpConnection(const QString & ip, int port, const QString & lgUser, const QString & lgPass, int transfermode, int connectmode)
{
    CurlFtp cFtp(this);
    // char url[512] = {0};
    QString url = QString("ftp://%1:%2").arg(ip).arg(port);
    //char usrPwd[100] = {0};
    QString usrPwd = QString("%1:%2").arg(lgUser).arg(lgPass);
    //sprintf(url, "ftp://%s:%d", ip.toStdString().c_str(), port);
    // sprintf(url, "sftp://%s", ip.toStdString().c_str(), port);
    //sprintf(usrPwd, "%s:%s", lgUser.toStdString().c_str(), lgPass.toStdString().c_str());
    cFtp.setFtpTransferMode(m_collectSet.ftp_transferMode);
    cFtp.setFtpConnectMode(m_collectSet.ftp_connectMode);

    return cFtp.connectToHost(url.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str());
}

bool CollectorBase::readSet()
{
    // 从数据库读取收集设置
    if (!DataBase::getInstance()->QueryCollectTask(m_collectSet))
    {
        QSLOG_ERROR("Fail to QueryCollectTask.");
        return false;
    }
	m_tUser.taskID = m_collectSet.dirID;
    m_tUser.connectId = m_collectSet.connectId;
    m_tUser.colTaskInfo = m_collectSet;
    // 读取分发用户设置
    if (!DataBase::getInstance()->QueryUserInfo(m_tUser))
    {
        QSLOG_ERROR("Fail to QueryUserInfo.");
        return false;
    }
    return true;
}

//void CollectorBase::recordLatestTime(const QString &dburl, const QString &dir, QString &latesttime)
//{
//    DataBase::getInstance()->queryDirLatestTime(dburl, dir, latesttime);
//}

//void CollectorBase::updateLatestTime(const QString &dburl, const QString &dir, const QString &latesttime)
//{
//    DataBase::getInstance()->updateDirLatestTime(dburl, dir, latesttime);
//}


bool CollectorBase::containsFile(list<string> &files, const QString &file)
{
    list<string>::const_iterator iter = std::find(files.begin(), files.end(), file.toLocal8Bit().toStdString());
    if (iter != files.end())
    {
        return true;
    }
    else
    {
        files.push_back(file.toLocal8Bit().toStdString());
        return false;
    }
}

bool CollectorBase::checkProcessFinished(const QString &dirId)
{
    QString strLockPath = qApp->applicationDirPath() + "/lock/";
    QDir oLockDir(strLockPath);
    if (!oLockDir.exists())
    {
        oLockDir.mkpath(strLockPath);
    }

    QLockFile oMem(strLockPath + dirId);
    if (oMem.tryLock())
    {
        return true;
    }
    return false;

    //QString strLockPath = qApp->applicationDirPath() + "/lock/";
    //QDir oLockDir(strLockPath);
    //QSharedMemory oMem(dirId);
    //if (!oMem.attach())
    //{
    //    return true;
    //}
    //return false;
}

QMap<QString, int> CollectorBase::queryLatestFileSize(const QString &url)
{
    QMap<QString, int> retMap;
    QString strDBPath = qApp->applicationDirPath() + "/work/record/" + m_collectSet.dirID + "/record.txt";
    // 文件格式说明
    // 首行为文件数
    // 从第二行开始，按照：文件全路径,文件大小 的方式进行存储
    QFile file(strDBPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open file failure:" + file.errorString());
        return retMap;
    }

    QTextStream stream(&file);
    QString strLine = stream.readLine();
    QString strContent = stream.readAll();
    int startIndex = 0;
    int endIndex = 0;
    while (-1 != (endIndex = strContent.indexOf("\n", startIndex)))
    {
        QString strLine = strContent.mid(startIndex, endIndex).trimmed();
        QStringList strData = strLine.split(",");
        QString filename = strData.at(0);
        int filesize = strData.at(1).toInt();
        retMap.insert(filename, filesize);
    }

    return retMap;
}

bool CollectorBase::updateLatestFileSize(const QString &url, const QMap<QString, int> &oFileSizeInfo)
{
    QString strDBPath = qApp->applicationDirPath() + "/work/record/" + m_collectSet.dirID + "/record.txt";
    // 文件格式说明
    // 首行为文件数
    // 从第二行开始，按照：文件全路径,文件大小 的方式进行存储
    QFile file(strDBPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QSLOG_ERROR("open file failure:" + file.errorString());
        return false;
    }

    QTextStream stream(&file);
    stream << oFileSizeInfo.size();
    for (QMap<QString, int>::const_iterator iter = oFileSizeInfo.begin(); iter != oFileSizeInfo.end(); ++iter)
    {
        stream << iter.key() << "," << iter.value();
    }
    return true;
}

QString CollectorBase::getTaskLockFilePath(const QString &dirId)
{
    QString strLockPath = qApp->applicationDirPath() + "/lock/";
    QDir oLockDir(strLockPath);
    if (!oLockDir.exists())
    {
        oLockDir.mkpath(strLockPath);
    }
    return strLockPath + dirId;
}

void CollectorBase::stoprcv(int exitcode, QProcess::ExitStatus status)
{
	m_bChildProcessRunning = false;
}
﻿#include "curlftp.h"
#include "record.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QUrl>

#ifdef _WIN32
#pragma comment(lib, "WS2_32")
#endif
//#include "ftpdownload.h"
#pragma warning(disable:4996)

// struct MemoryData
// {
//     char *memdata;
//     size_t size;
//
//     MemoryData()
//     {
//         memdata = NULL;
//         size = 0;
//     }
// };
void ftptest(string &localpath, string &filename)
{

}


//const string TmpNameSuffix = ".tmp";

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

CurlFtp::CurlFtp(CollectorBase *pBase)
//: m_bConnected(false)
{
    initcurl();

    m_pCoBase = pBase;

    if (NULL != m_pCoBase)
    {
        connect(this, SIGNAL(emitLog(const QString &, int)), m_pCoBase, SLOT(emitLog(const QString &, int)));
    }


    m_pSourceCurl = curl_easy_init();

    m_pDestCurl = curl_easy_init();

    m_pRemoveDestCurl = curl_easy_init();
}

CurlFtp::~CurlFtp()
{
    uninitcurl();

    if (NULL != m_pSourceCurl)
    {
        curl_easy_cleanup(m_pSourceCurl);
        m_pSourceCurl = NULL;
    }

    if (NULL != m_pDestCurl)
    {
        curl_easy_cleanup(m_pDestCurl);
        m_pDestCurl = NULL;
    }

    if (NULL != m_pRemoveDestCurl)
    {
        curl_easy_cleanup(m_pRemoveDestCurl);
        m_pRemoveDestCurl = NULL;
    }
}

int CurlFtp::setCommOpt()
{
    CURLcode ret = CURLE_OK;

    m_pCurl = curl_easy_init();
    if (m_pCurl != NULL)
    {
        char ftpurl[260] = {0};
        sprintf(ftpurl, "ftp://%s:%s@%s:%d/%s", m_strUser.c_str(), m_strPwd.c_str(), m_strIP.c_str(), m_nPort, m_strRoot.c_str());

        curl_easy_setopt(m_pCurl, CURLOPT_URL, ftpurl);
        // 不下载文件
        curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1L);

        curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL,1L);
    }

    return 0;
}

int CurlFtp::getNewFiles(FileInfoList &fileList, CDirRecord &in_record)
{
    // 每次都去连接
    //char url[512] = {0};
    //char usrpwd[100] = {0};
    QString url = QString("ftp://%1:%2%3/").arg(m_strIP.c_str()).arg(m_nPort).arg(m_strRoot.c_str());
    //sprintf(url, "ftp://%s:%d%s/", m_strIP.c_str(), m_nPort, m_strRoot.c_str());
    QSLOG_DEBUG("WE WILL LOOK FOR:" + url);
    // modified by liubojun
    // sprintf(url, "sftp://%s", m_strIP.c_str(), m_nPort);
    QString usrpwd = QString("%1:%2").arg(m_strUser.c_str()).arg(m_strPwd.c_str());
    // sprintf(usrpwd, "%s:%s", m_strUser.c_str(), m_strPwd.c_str());
    if (!connectToHost(url.toLocal8Bit().toStdString().c_str(), usrpwd.toLocal8Bit().toStdString().c_str()))
        // if (!connectToHost(url, usrpwd.toLocal8Bit().toStdString().c_str()))
    {
        emit done();
        QSLOG_ERROR("connectToHost error." + url);
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////

    m_lstDirs.clear();
    DIRLEVEL oDirLevel;
    oDirLevel.dir = QString::fromStdString(m_strRoot);
    oDirLevel.level = 0;	// 根目录层级为0
    m_lstDirs.append(oDirLevel);
    m_oSubDirFilter.init();

    while (!m_lstDirs.isEmpty())
    {
        if (!m_pCoBase->m_bRun)
        {
            break;
        }

        m_strCurDir = m_lstDirs.takeFirst();
        //QSLOG_DEBUG(m_strCurDir);
        if (m_strCurDir.dir.length() >= 1 && m_strCurDir.dir.right(1) != "/")
        {
            m_strCurDir.dir += "/";
        }
        //QSLOG_DEBUG(m_strCurDir);
//          QDateTime qdtime = QDateTime::fromTime_t(m_pCoBase->m_pTsctTime->mapDirTime[m_strCurDir.toStdString()]);
//          m_strCurDirLastTime = qdtime.toString("yyyyMMddHHmmss").toStdString();
        listFiles(m_strCurDir.dir.toLocal8Bit().data(), fileList, in_record);
    }

    emit done();
    return 0;
}

bool CurlFtp::connectToHost(const char *url, const char *user_pwd, int timeout)
{
    if (m_pCurl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }

    curl_easy_reset(m_pCurl);

    // struct curl_slist *headerlist = NULL;

    if (1 == ftp_transfermode)
    {
        if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_TRANSFERTEXT, 1L))
        {
            QSLOG_ERROR("curl_easy_setopt error.");
            return false;
        }
        //string cmd = "TYPE A";
        //headerlist = curl_slist_append(headerlist, cmd.c_str());
    }
    else
    {
        if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_TRANSFERTEXT, 0))
        {
            QSLOG_ERROR("curl_easy_setopt error.");
            return false;
        }
        //string cmd = "TYPE I";
        //headerlist = curl_slist_append(headerlist, cmd.c_str());
    }


    /* pass in that last of FTP commands to run after the transfer */
    //curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, headerlist);
    if (1 == ftp_connectmode)
    {
        if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_FTPPORT, "-"))
        {
            QSLOG_ERROR("curl_easy_setopt error.");
            return false;
        }
    }



    // CURL *curl = curl_easy_init();
    // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);

    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_URL, url))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, user_pwd))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }
    // modified by liubojun.
    // 20170828,ftp大目录，轮询速度较慢
    if (-1 != timeout)
    {
        if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, timeout))
        {
            QSLOG_ERROR("curl_easy_setopt error.");
            return false;
        }
    }

    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }
    // curl_easy_setopt(m_pCurl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_DEFAULT);
//#ifndef DISABLE_SSH_AGENT
//    curl_easy_setopt(m_pCurl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_AGENT);
//#endif
    // curl_easy_setopt(m_pCurl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    //curl_easy_setopt(m_pCurl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }

    // modified by liubojun @2017-10-28,没有这两句话会出问题
    struct MemoryData listInfo;
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void *)&listInfo))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }

    CURLcode res = curl_easy_perform(m_pCurl);
    if (CURLE_OK != res)
    {
        // modified by liubojun,发起两次链接，针对sftp，第一次失败，第二次成功，原因未知
        //res = curl_easy_perform(m_pCurl);
        //if (CURLE_OK != res)
        //{
        QString strLogInfo(QString("connectToHost error: %1").arg(curl_easy_strerror(res)));
        QSLOG_ERROR(strLogInfo);
        emit emitLog(strLogInfo, BAD);
        return false;
        //}
    }
    //curl_slist_free_all(headerlist);

    return true;
}

void CurlFtp::listFiles(const string &strDir, FileInfoList &fileList, CDirRecord &in_record)
{
    // 暂时注释
    CURLcode res;

    struct MemoryData listInfo;
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void *)&listInfo))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return;
    }
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return;
    }
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return;
    }
    // 切换到strDir目录
    // string strCmd = string("CWD ") + "//x_rada";
    //curl_easy_setopt(m_pCurl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD);
    //curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, strCmd.c_str());
    //res = curl_easy_perform(m_pCurl);

    //string strCmd = string("CWD ") + strDir;
    //struct curl_slist *headerlist = NULL;
    //headerlist = curl_slist_append(headerlist, strCmd.c_str());
    ///* pass in that last of FTP commands to run after the transfer */
    //curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, headerlist);
    //res = curl_easy_perform(m_pCurl);
    //if (res != CURLE_OK)
    //{
    //    QSLOG_ERROR(QString::fromLocal8Bit("切换目录失败。"));
    //    curl_slist_free_all(headerlist);
    //    free(listInfo.memdata);
    //    return;
    //}
    //char url[512] = { 0 };
    //sprintf(url, "ftp://%s:%d%s/", m_strIP.c_str(), m_nPort, strDir.c_str());
    QString url = QString("ftp://%1:%2%3").arg(m_strIP.c_str()).arg(m_nPort).arg(strDir.c_str());
    //QSLOG_DEBUG(url);
    if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str()))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return;
    }

    // list当前目录
    //curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "MLSD");
    //res = curl_easy_perform(m_pCurl);
    bool bFtpSupportMLSD = true;
    //if (res != CURLE_OK)
    {
        bFtpSupportMLSD = false;
        //QSLOG_ERROR(QString("MLSD error: %1, list dir: %2").arg(curl_easy_strerror(res)).arg(QString::fromLocal8Bit(strCmd.c_str())));
        //QSLOG_DEBUG(QString::fromLocal8Bit("使用MLSD命名返回错误,尝试使用LIST命名"));
        if (CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "LIST"))
        {
            QSLOG_ERROR("curl_easy_setopt error");
            return;
        }
        res = curl_easy_perform(m_pCurl);
        if (res != CURLE_OK)
        {
            //free(listInfo.memdata);
            QSLOG_ERROR("curl_easy_perform error" + QString(curl_easy_strerror(res)));
            return;
        }
    }
    // 解析MLSD的返回内容
    //QString strInfo = QString::fromLocal8Bit(listInfo.memdata);
    QString strInfo(listInfo.memdata);		//不同的机器可能乱码??
    //QSLOG_DEBUG(strInfo);
    //free(listInfo.memdata);

    // QString strDBPath = qApp->applicationDirPath()+ "/work/record/" + m_pCoBase->m_collectSet.dirID + "/record.index";

    // 当前目录数据库中记录的最后处理时间
    // QString iLatestTime("");

    QSLOG_DEBUG(QString::fromLocal8Bit("开始处理目录:%1， 收集时间范围:%2, 目录最后处理时间记录标识:%3").arg(QString::fromLocal8Bit(strDir.c_str())).arg(m_pCoBase->m_collectSet.col_timerange).arg(m_pCoBase->m_collectSet.recordLatestTime));

    bool bHasFileUpdate = false;
    //if (m_pCoBase->m_collectSet.recordLatestTime)
    //{
    //    //in_record.loadLatestFileSize();
    //    //m_pCoBase->recordLatestTime(strDBPath, QString::fromLocal8Bit(strDir.c_str()), iLatestTime);
    //    //QSLOG_DEBUG(QString::fromLocal8Bit("目录:%1最后修改时间为:%2").arg(QString::fromLocal8Bit(strDir.c_str())).arg(iLatestTime));
    //}

    QString nMdfTime = parseMlsdInfo(QString::fromLocal8Bit(strDir.c_str()), strInfo, fileList, m_lstDirs, in_record, bFtpSupportMLSD);

    //if (m_pCoBase->m_collectSet.recordLatestTime)
    //{
    //    QSLOG_DEBUG(QString::fromLocal8Bit("目录最后修改时间:%1").arg(nMdfTime));
    //    //if (nMdfTime > iLatestTime)
    //    //{
    //    //    m_pCoBase->updateLatestTime(strDBPath, QString::fromLocal8Bit(strDir.c_str()), nMdfTime);
    //    //}
    //}


    //     if (m_strCurDirNewTime > m_strCurDirLastTime)
    //     {
    //         QDateTime time = QDateTime::fromString(QString::fromStdString(m_strCurDirNewTime), "yyyyMMddhhmmss");
    //         m_pCoBase->m_pTsctTime->mapDirTime[m_strCurDir.toStdString()] = time.toTime_t();	//转为整形
    //}
}

QString CurlFtp::parseMlsdInfo(const QString &rootPath, const QString &info, FileInfoList &fileList, QList<DIRLEVEL> &dirList, CDirRecord &in_record, bool bFtpSupportMSDL)
{
    // 当前目录的最新时间列表
    //QString strFileListPath = qApp->applicationDirPath() + "/work/record/" + m_pCoBase->m_collectSet.dirID + "/latestFileList.xml";
    //list<string> t_oLatestFileList = m_pCoBase->initilizeLatestFileLists(strFileListPath);
    // 新的最新文件列表
    //list<string> t_oNewLatestFiles = t_oLatestFileList;
    bool listUpdate = false;
    // modified by liubojun @20170606
    //time_t iRecordMaxTime = iLatestTime;



    QString iLastModifiedTime("");

    // QStringList lstLines = info.split("\r\n");
    QStringList lstLines = info.split("\n");
    for (int i=0; i<lstLines.size(); ++i)
    {
        QString strLine = lstLines.at(i).trimmed();
        if (strLine.isEmpty())
        {
            continue;
        }

        CListInfo oneInfo;
        // modified by liubojun
        QStringList lstParts;
        if (bFtpSupportMSDL)
        {
            lstParts = strLine.split(";");
        }
        else
        {
            lstParts = strLine.split(" ");
        }

        // QStringList lstParts = strLine.split(" ");
        QStringList tmpParts;
        foreach (QString str, lstParts)
        {
            if (!str.isEmpty())
            {
                tmpParts.append(str);
            }
        }
        lstParts = tmpParts;

        if (bFtpSupportMSDL)
        {
            for (int j=0; j<lstParts.size(); ++j)
            {
                QString strPart = lstParts.at(j).trimmed().toUpper();
                int nPos = strPart.indexOf("=");
                if (nPos == -1)		//文件名或目录名
                {
                    oneInfo.strFileName = lstParts.at(j).trimmed();
                }
                else
                {
                    if (strPart.indexOf("TYPE") != -1)	//文件类型
                    {
                        if (strPart.right(strPart.size() - nPos -1) == "DIR")
                        {
                            oneInfo.nType = 2;
                        }
                        else if (strPart.right(strPart.size() - nPos -1) == "FILE")
                        {
                            oneInfo.nType = 1;
                        }
                    }
                    else if (strPart.indexOf("MODIFY") != -1)
                    {
                        oneInfo.strMdfyTime = strPart.right(strPart.size() - nPos -1);
                    }
                    else if (strPart.indexOf("SIZE") != -1)
                    {
                        oneInfo.nFileSize = strPart.right(strPart.size() - nPos -1).toLongLong();
                    }
                }
            }
        }
        else
        {
            // 日期 时间 大小 文件名
            if (lstParts.size() == 4)
            {
                //QSLOG_ERROR(QString::fromLocal8Bit("发现特殊ftp返回..."));

                process4Ftp(oneInfo, lstParts);
            }
            // -rw-r--r--    1 0        0        61456824 May 10 06:59 Z_NAFP_C_BABJ_2017050912_P_CAMS-GRAPES-ACHN-03.grd
            else if (lstParts.size() == 9)
            {
                // 4 是大小， 5是月份，6是日期，7是时分，8是文件名
                process9Ftp(oneInfo, lstParts);
            }


        }

        // ftp获取的时间格式不定，如果使用增量轮训，可能会出现问题，所以在ftp收集模式下，应该禁用根据收集时间段收集配置
		QString nMdfyTime("");
		if (oneInfo.strMdfyTime.length() >= 14)
		{
			nMdfyTime = oneInfo.strMdfyTime.mid(0, 14);
		}
		else
		{
			QSLOG_ERROR("oneInfo.strMdfyTime length is incorrect.");
			continue;
		}

        // LIST命令只能获取到分钟，所以出现分钟一样的文件，应该与目标目录下进行比较
        //if (nMdfyTime > iLastModifiedTime)
        //{
        //    iLastModifiedTime = nMdfyTime;
        //}

        if (oneInfo.nType == 1)
        {
            FileInfo fInfo;
            fInfo.strFileName = oneInfo.strFileName.toLocal8Bit().data();
            fInfo.nFileSize = oneInfo.nFileSize;
            //if (oneInfo.strMdfyTime.length() >= 14)
            //{
            //    fInfo.strMdyTime = oneInfo.strMdfyTime.toStdString().substr(0, 14);
            //}
            //else
            //{
            //    QSLOG_ERROR("oneInfo.strMdfyTime length is incorrect.");
            //    continue;
            //}


            // 增加针对.tmp临时文件结尾的收集文件进行过滤
            if (QString::fromLocal8Bit(fInfo.strFileName.c_str()).toLower().endsWith(".tmp"))
            {
                continue;
            }
            // 新增加判断条件（记录目录最后修改时间，此处需要判断当前文件时间是否大于等于当前目录的保存上次处理最后修改时间，如果小于则不对该文件进行处理）
            if (m_pCoBase->m_collectSet.recordLatestTime)
            {
                in_record.updateLatestFileSize(rootPath, QString::fromLocal8Bit(fInfo.strFileName.c_str()), fInfo.strMdyTime.c_str(), fInfo.nFileSize);

                if (!in_record.checkIsNewFile(rootPath, QString::fromLocal8Bit(fInfo.strFileName.c_str()), fInfo.strMdyTime.c_str(), fInfo.nFileSize))
                {
                    continue;
                }
                //in_record.updateLatestFileSize(QString::fromLocal8Bit(fInfo.strFileName.c_str()), fInfo.strMdyTime.c_str(), fInfo.nFileSize);
                //if (!iLatestTime.isEmpty() && nMdfyTime <= iLatestTime)
                //{
                //    continue;
                //}

            }

            // 必须大于设置的时间范围才收集

            if (m_pCoBase->m_collectSet.col_timerange != -1)
            {
                // 分钟转秒
                if (nMdfyTime < QDateTime::currentDateTime().addSecs(-m_pCoBase->m_collectSet.col_timerange*60).toString("yyyyMMddhhmmss"))
                {
                    continue;
                }
            }

            // if (fInfo.strMdyTime)

            if (m_pCoBase->filterFileName(fInfo)/* && fInfo.strMdyTime > m_strCurDirLastTime*/)
            {
                fInfo.strFilePath = (m_strCurDir.dir + oneInfo.strFileName).toLocal8Bit().data();
                fInfo.nFileSize = oneInfo.nFileSize;
                fileList.push_back(fInfo);

                //                 if (m_strCurDirNewTime < fInfo.strMdyTime)
                //                 {
                //                     m_strCurDirNewTime = fInfo.strMdyTime;
                //                 }
            }
        }
        else if (oneInfo.nType == 2 && m_subDirFlag)
        {
            //
            if (oneInfo.strFileName == "." || oneInfo.strFileName == "..")
            {
                continue;
            }
            // 如果需要遍历子目录
            QString strPath = m_strCurDir.dir + oneInfo.strFileName + "/";
            DIRLEVEL oDirLevel;
            oDirLevel.dir = strPath;
            oDirLevel.level = m_strCurDir.level + 1;

            // modified by liubojun @20180222,针对北京民航张睿之现场反馈问题提出的新需求
            // 在此处加入过滤条件，支持针对子目录的模糊匹配
            if (m_oSubDirFilter.match(m_strSubDirTemplate, oneInfo.strFileName, oDirLevel.level))
            {
                dirList.append(oDirLevel);
            }
            else
            {
                QSLOG_DEBUG(QString("subdir %1 is not match the rule").arg(oDirLevel.dir));
            }
        }
    }

    //if (m_pCoBase->m_collectSet.recordLatestTime)
    //{
    //    if (listUpdate)
    //    {
    //        m_pCoBase->updateLatestFileLists(strFileListPath, t_oNewLatestFiles);
    //    }
    //}

    return iLastModifiedTime;
}

void CurlFtp::setHostPort(const QString &strIP, const int &nPort)
{
    m_strIP = strIP.toStdString();
    m_nPort = nPort;
}

void CurlFtp::setUserPwd(const QString &strUser, const QString &strPwd)
{
    m_strUser = strUser.toStdString();
    m_strPwd = strPwd.toStdString();
}

void CurlFtp::setRootPath(const QString &strPath)
{
    m_strRoot = strPath.toStdString();
}

void CurlFtp::initcurl()
{

    m_pCurl = curl_easy_init();
    if (m_pCurl == NULL)
    {
        QSLOG_ERROR("curl_init error.");
        exit(-1);
    }
}

void CurlFtp::uninitcurl()
{
    //QSLOG_INFO("uninit");
    curl_easy_cleanup(m_pCurl);
    m_pCurl = NULL;
}

int CurlFtp::downloadFile(const char *url, const char *user_pwd, FileData *fileData)
{
    //CURL *curl = m_pCurl;
    //if (curl == NULL)
    //{
    //    QSLOG_ERROR("m_pCurl is null");
    //    return -1;
    //}

    //// 重置
    //curl_easy_reset(m_pCurl);

    CURL *curl = m_pSourceCurl;
    // CURL *curl = curl_easy_init();
    if (NULL == curl)
    {
        QSLOG_ERROR(QStringLiteral("初始化curl失败!"));
        return -1;
    }
    // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);

    // 指定全路径url
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_URL, url))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (user_pwd != NULL)
    {
        if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd))
        {
            QSLOG_ERROR("curl_easy_setopt error");
            return -1;
        }
    }
    if (1 == ftp_transfermode)
    {
        if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1L))
        {
            QSLOG_ERROR("curl_easy_setopt error");
            return -1;
        }
        //string cmd = "TYPE A";
        //headerlist = curl_slist_append(headerlist, cmd.c_str());
    }
    else
    {
        if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 0))
        {
            QSLOG_ERROR("curl_easy_setopt error");
            return -1;
        }
        //string cmd = "TYPE I";
        //headerlist = curl_slist_append(headerlist, cmd.c_str());
    }


    /* pass in that last of FTP commands to run after the transfer */
    //curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, headerlist);
    if (1 == ftp_connectmode)
    {
        if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_FTPPORT, "-"))
        {
            QSLOG_ERROR("curl_easy_setopt error");
            return -1;
        }
    }
    // 设置下载文件的回调函数
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInFileFun))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    // 设置传递给回调函数的对象指针
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileData))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }

    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error.");
        return false;
    }

    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;

    }

    /* QUrl ourl(QString::fromLocal8Bit(url));
    QString strFile = ourl.toLocalFile();
    QFile f(strFile);
    qint64 size = f.size();
    QSLOG_DEBUG(QString("%1").arg(f.size()));
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, 0);*/
    //--进度条功能--
    //struct myprogress prog;
    //prog.lastruntime = 0;
    //prog.curl = curl;
    //curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    //curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------

    QSLOG_INFO(QString::fromLocal8Bit("开始从%1下载文件").arg(QString::fromLocal8Bit(url)));
    CURLcode res = curl_easy_perform(curl);

    // 关闭文件
    if (fileData->stream)
    {
        fclose(fileData->stream);
    }

    if (CURLE_OK != res)
    {
        QString strLogInfo(QString("downloadFile error: %1").arg(curl_easy_strerror(res)));
        QSLOG_ERROR(strLogInfo);
        //curl_easy_cleanup(curl);
        emit emitLog(strLogInfo, BAD);
        //m_pCoBase->emitLog(strLogInfo, BAD);
        return -1;
    }
    QSLOG_INFO(QString::fromLocal8Bit("文件下载到:%1成功").arg(fileData->filename));
    //curl_easy_cleanup(curl);

    // 先做目录及的剪切
    // pBase->
    return 0;
}


int CurlFtp::uploadFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename, const char *sendsuffix)
{
    if (!connectToHost(url, user_pwd))
    {
        return -1;
    }

    //CURL *curl = m_pCurl;
    //if (curl == NULL)
    //{
    //    return -1;
    //}
    //curl_easy_reset(m_pCurl);	// 重置

    CURL *curl = curl_easy_init();
    // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL,1L);
    struct curl_slist *headerlist = NULL;
    CURLcode res = CURLE_OK;
    // 删除已有文件
    deleteFtpFile(url, user_pwd, filename);

    char * const pStart = memData->memdata;	//记录起始位置
    size_t orgSize = memData->size;			//记录原始大小
    string cmdTmpName = "RNFR " + filename + sendsuffix;
    string cmdOrgName = "RNTO " + filename;
    string strUrl = url;
    strUrl += filename + sendsuffix;

    headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
    headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    curl_easy_setopt(curl, CURLOPT_READDATA, memData);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, UploadFromMemFun);
    /* pass in that last of FTP commands to run after the transfer */
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
    //curl_easy_cleanup(curl);
    memData->memdata = pStart;		//还原起始位置
    memData->size = orgSize;
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_cleanup(curl);
    return 0;
}

int CurlFtp::uploadFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *tmpsuffix)
{
    //if (m_pCurl == NULL)
    //{
    //    QSLOG_ERROR("m_pCurl is null.");
    //    return -1;
    //}
    //QSLOG_INFO("upload");
    CURL *pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL,1L);
    if (NULL == pCurl)
    {
        QSLOG_ERROR(QString::fromLocal8Bit("初始化curl失败."));
        return -1;
    }
    // QSharedPointer<CURL> autoRelease(pCurl, curl_easy_cleanup);
    // curl_easy_reset(m_pCurl);
    // 删除已有文件

    //QString strRemoteFile(url);
    //strRemoteFile.append(filename.c_str());
    //QUrl oUrl(strRemoteFile);
    //strRemoteFile = oUrl.path();
    //QFile ff(strRemoteFile);
    //if (!ff.remove())
    //{
    //	//QSLOG_ERROR(QString::fromLocal8Bit("删除文件:%1失败").arg(strRemoteFile));
    //	//return -1;
    //}
    if (-1 == deleteLocalFile(url, user_pwd, filename))
    {
        //return -1;
    }

    FILE *pfile = fopen(localPath, "rb");
    if (pfile == NULL)
    {
        QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    QSharedPointer<FILE> autoClose(pfile, fclose);
    // 获取文件大小
    fseek(pfile, 0, SEEK_END);
    long fsize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);

    CURLcode res = CURLE_OK;
    string strUrl = url;
    strUrl += filename + tmpsuffix;
    res = curl_easy_setopt(pCurl, CURLOPT_UPLOAD, 1);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("curl_easy_setopt error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    res = curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("curl_easy_setopt error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    res = curl_easy_setopt(pCurl, CURLOPT_USERPWD, user_pwd);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("curl_easy_setopt error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    res = curl_easy_setopt(pCurl, CURLOPT_READDATA, pfile);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("curl_easy_setopt error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    res = curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, ReadFromFile);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("curl_easy_setopt error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    res = curl_easy_setopt(pCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        curl_easy_cleanup(pCurl);
        return -1;
    }

    QSLOG_INFO(QString::fromLocal8Bit("开始上传文件:%1").arg(QString::fromLocal8Bit(strUrl.c_str())));
    //--进度条功能--
    //     struct myprogress prog;
    //     prog.lastruntime = 0;
    //     prog.curl = m_pCurl;
    //     curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, progressFun);
    //     curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, &prog);
    //     curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(pCurl);

    if (CURLE_OK != res)
    {
        QString logInfo(QString("uploadFile error: errcode = %1, reason = %2").arg(res).arg(curl_easy_strerror(res)));
        QSLOG_ERROR(logInfo);
        emit emitLog(logInfo, BAD);
        //m_pCoBase->emitLog(logInfo, BAD);
        curl_easy_cleanup(pCurl);
        return -1;
    }
    QSLOG_INFO(QString::fromLocal8Bit("上传文件:%1成功").arg(QString::fromLocal8Bit(strUrl.c_str())));
    curl_easy_cleanup(pCurl);

    return 0;
}

int CurlFtp::deleteFtpFile(const char *url, const char *user_pwd, const string &filename)
{
    // 如果远端Ftp服务器上已经存在了该文件，那么需要删除该文件

    //char szCmd[1024] = "DELE ";
    //strcat(szCmd, filename.c_str());
    QString szCmd = QString("DELE %1").arg(filename.c_str());
    curl_easy_setopt(m_pRemoveDestCurl, CURLOPT_URL, url);
    curl_easy_setopt(m_pRemoveDestCurl, CURLOPT_USERPWD, user_pwd);
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, szCmd.toLocal8Bit().toStdString().c_str());
    curl_easy_setopt(m_pRemoveDestCurl, CURLOPT_POSTQUOTE, headerlist);

    // modified by liubojun @2017-10-28,没有这两句话会出问题
    struct MemoryData listInfo;
    curl_easy_setopt(m_pRemoveDestCurl, CURLOPT_WRITEDATA, (void *)&listInfo);
    curl_easy_setopt(m_pRemoveDestCurl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);

    CURLcode res = curl_easy_perform(m_pRemoveDestCurl);
    if (CURLE_OK != res)
    {
        /* we failed */
        QSLOG_ERROR(QString::fromLocal8Bit("remote file exists, remove remote file %1 failure, error reason: %2").arg(filename.c_str()).arg(curl_easy_strerror(res)));
        curl_slist_free_all(headerlist);
        return -1;
    }
    curl_slist_free_all(headerlist);
    return 0;
}

int CurlFtp::deleteLocalFile(const char *url, const char *user_pwd, const string &filename)
{
    QString strDir = QUrl(url).toLocalFile();
    QFile file(strDir + filename.c_str());
    if (!QFile::remove(strDir + filename.c_str()))
    {
        //QSLOG_ERROR(QString::fromLocal8Bit("remove file err:") + file.errorString());
        return -1;
    }
    return 0;
}

bool CurlFtp::getFileSize(const char *url, const char *user_pwd, const string &filename, double &fileSize)
{
    // CURL *curl = curl_easy_init();
    CURL *curl = m_pCurl;
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }
    fileSize = 0.0;
    string strUrl = url;
    //strUrl += filename;
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL,1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    //---File模式必须这样写---
    //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
    //curl_easy_setopt(curl, CURLOPT_HEADER, TRUE);
    //------
    // modified by liubojun @2017-10-28,没有这两句话,不在控制台下面运行会出问题
    struct MemoryData listInfo;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&listInfo);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
    {
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
        if((CURLE_OK == res) && (fileSize > 0.0))
        {
            //curl_easy_cleanup(curl);
            return true;
        }
    }

    //curl_easy_cleanup(curl);
    return false;
}


int CurlFtp::uploadFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix, int transferMode)
{
    QSLOG_DEBUG("begin upload file to ftp");
    // 连接ftp服务器
    //if (!TestConnection(url, user_pwd))
    //{
    //	return -1;
    //}

    // 删除已有文件
    //deleteFtpFile(url, user_pwd, filename);

    // 打开本地文件，并获取文件大小
    FILE *pfile = fopen(localPath, "rb");

    if (pfile == NULL)
    {
        QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
        return -1;
    }
    fseek(pfile, 0, SEEK_END);
    long filesize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);
    QSharedPointer<FILE> autoClose(pfile, fclose);
    //fseek(pfile, 0, SEEK_END);
    //long fsize = ftell(pfile);
    //fseek(pfile, 0, SEEK_SET);
    /*QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open file: " + QString(localPath) + " failure");
        return -1;
    }*/

    //CURL *curl = m_pCurl;
    //if (curl == NULL)
    //{
    //    return -1;
    //}
    //curl_easy_reset(m_pCurl);
    // 当前m_pCurl保存的是目标目录的curl对象，所以应该复用
    CURL *curl = m_pDestCurl;
    // CURL *curl = curl_easy_init();
    // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    struct curl_slist *headerlist = NULL;
    CURLcode res = CURLE_OK;

    string cmdTmpName = "RNFR " + filename + sendsuffix;
    string cmdOrgName = "RNTO " + filename;
    string strUrl = url;
    strUrl += filename + sendsuffix;

    headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
    if (NULL == headerlist)
    {
        QSLOG_ERROR("curl_slist_append error");
        return -1;
    }
    headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    if (NULL == headerlist)
    {
        QSLOG_ERROR("curl_slist_append error");
        return -1;
    }

    QSharedPointer<curl_slist> autoDelete(headerlist, curl_slist_free_all);

    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_UPLOAD, 1))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_READDATA, pfile))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFromFile))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filesize))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }
    if (CURLE_OK != curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L))
    {
        QSLOG_ERROR("curl_easy_setopt error");
        return -1;
    }

    if (1 == transferMode)
    {
        curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
    }
    //--进度条功能--
    // 	struct myprogress prog;
    // 	prog.lastruntime = 0;
    // 	prog.curl = curl;
    // 	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    // 	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    // 	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(curl);
    //curl_slist_free_all(headerlist);

    if (CURLE_OK != res)
    {
        QString logInfo(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        QSLOG_ERROR(logInfo);
        emit emitLog(logInfo, BAD);
        //m_pCoBase->emitLog(logInfo, BAD);
        //curl_easy_cleanup(curl);
        return -1;
    }

    //curl_easy_cleanup(curl);
    QSLOG_DEBUG("end upload file to ftp");
    return 0;
}

int CurlFtp::conputFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix)
{
    double dHasSend = 0.0;
    string strTmpName = filename + sendsuffix;
    if (getFileSize(url, user_pwd, strTmpName, dHasSend))
    {
        //if (m_pCurl == NULL)
        //{
        //    QSLOG_ERROR("m_pCurl is null.");
        //    return -1;
        //}
        //curl_easy_reset(m_pCurl);
        CURL *curl = curl_easy_init();
        // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        string strUrl = url;
        strUrl += filename + sendsuffix;
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
        //////////////////////////////////////////////////////////////////////////
        // 断点续传（另一个方式）
        FILE *pfile = fopen(localPath, "rb");
        if (pfile == NULL)
        {
            QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
            curl_easy_cleanup(curl);
            return -1;
        }
        fseek(pfile, 0, SEEK_END);
        long fsize = ftell(pfile);
        fseek(pfile, 0, SEEK_SET);

        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > fsize)
        {
            tHasSend = fsize;
        }

        //fseek(pfile, tHasSend, SEEK_SET);
        curl_easy_setopt(curl, CURLOPT_APPEND, 1);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, tHasSend);
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(curl, CURLOPT_READDATA, pfile);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFromFile);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)fsize);
        CURLcode res = curl_easy_perform(curl);

        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            curl_easy_cleanup(curl);
            return -1;
        }
        curl_easy_cleanup(curl);
    }
    else
    {
        return uploadFileToDir(url, user_pwd, filename, localPath, sendsuffix);
    }

    return 0;
}

int CurlFtp::conputFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix)
{
    //if (!connectToHost(url, user_pwd))
    //{
    //    return -1;
    //}

    double dHasSend = 0.0;
    string strTmpName = filename + sendsuffix;
    if (getFileSize(url, user_pwd, strTmpName, dHasSend))
    {
        //CURL *curl = m_pCurl;
        //if (curl == NULL)
        //{
        //    QSLOG_ERROR("m_pCurl is null.");
        //    return -1;
        //}
        // curl_easy_reset(m_pCurl);
        CURL *curl = curl_easy_init();
        // QSharedPointer<CURL> autoRelease(curl, curl_easy_cleanup);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        // 删除已有文件
        deleteFtpFile(url, user_pwd, filename);

        string cmdTmpName = "RNFR " + filename + sendsuffix;
        string cmdOrgName = "RNTO " + filename;
        string strUrl = url;
        strUrl += filename + sendsuffix;
        struct curl_slist *headerlist = NULL;
        headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
        headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
        //////////////////////////////////////////////////////////////////////////
        // 断点续传
        FILE *pfile = fopen(localPath, "rb");
        if (pfile == NULL)
        {
            QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
            curl_easy_cleanup(curl);
            return -1;
        }
        fseek(pfile, 0, SEEK_END);
        long fsize = ftell(pfile);

        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > fsize)
        {
            tHasSend = fsize;
        }

        fseek(pfile, tHasSend, SEEK_SET);

        curl_easy_setopt(curl, CURLOPT_APPEND, 1);
        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM, tHasSend);
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
        curl_easy_setopt(curl, CURLOPT_READDATA, pfile);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFromFile);
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headerlist);

        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            curl_easy_cleanup(curl);
            return -1;
        }
        curl_easy_cleanup(curl);
    }
    else
    {
        return uploadFileToFtp(url, user_pwd, filename, localPath, sendsuffix, 0);
    }

    return 0;
}

bool CurlFtp::TestConnection(const char *url, const char *user_pwd)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);	// 目录不存在自动创建
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);

    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK != res)
    {
        QSLOG_INFO(QString("connectToHost error: %1").arg(curl_easy_strerror(res)));
        curl_easy_cleanup(curl);
        return false;
    }
    curl_easy_cleanup(curl);
    return true;
}

void CurlFtp::process4Ftp(CListInfo &oneInfo, const QStringList &lstParts)
{
    if (lstParts[2].toUpper() != "<DIR>")
    {

        oneInfo.strFileName = lstParts[3];
        oneInfo.nFileSize = lstParts[2].toLongLong();
        oneInfo.nType = 1;
        //05-07-17 12:23AM

        // oneInfo.strMdfyTime = QDateTime::fromString(lstParts[0] + lstParts[1] , "MM-dd-yyhh:mm").toString("yyyyMMddhhmmss");
        if (lstParts[1].length() >= 8 && lstParts[1].mid(5, 2).toUpper() == "AM")
        {

            oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt(), lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
        }
        else
        {
            if (lstParts[1].length() >= 8 && lstParts[1].mid(0, 2).toInt() == 12)
            {
                // 如果到下午12点，正好为PM
                oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt(), lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
            }
            else
            {
                oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt()+12, lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
            }

        }

    }
    else
    {
        oneInfo.strFileName = lstParts[3];
        oneInfo.nType = 2;
        //QSLOG_ERROR(QString::fromLocal8Bit("ftp 目前暂时不考虑子目录问题."));
    }
}

void CurlFtp::process9Ftp(CListInfo &oneInfo, const QStringList &lstParts)
{
    //1 = "Jan"
    //2 = "Feb"
    //3 = "Mar"
    //4 = "Apr"
    //5 = "May"
    //6 = "Jun"
    //7 = "Jul"
    //8 = "Aug"
    //9 = "Sep"
    //10 = "Oct"
    //11 = "Nov"
    //12 = "Dec"
    // -rw-r--r--    1 0        0        61456824 May 10 06:59 Z_NAFP_C_BABJ_2017050912_P_CAMS-GRAPES-ACHN-03.grd
    // 4 是大小， 5是月份，6是日期，7是时分，8是文件名
    oneInfo.strFileName = lstParts[8];
    oneInfo.nFileSize = lstParts[4].toLongLong();

    if (lstParts[0].startsWith("d"))
    {
        oneInfo.nType = 2;
    }
    else
    {
        oneInfo.nType = 1;
    }


    int month = 0;
    if (lstParts[5] == "Jan")
    {
        month = 1;
    }
    else if (lstParts[5] == "Feb")
    {
        month = 2;
    }

    else if (lstParts[5] == "Mar")
    {
        month = 3;
    }
    else if (lstParts[5] == "Apr")
    {
        month = 4;
    }
    else if (lstParts[5] == "May")
    {
        month = 5;
    }
    else if (lstParts[5] == "Jun")
    {
        month = 6;
    }
    else if (lstParts[5] == "Jul")
    {
        month = 7;
    }
    else if (lstParts[5] == "Aug")
    {
        month = 8;
    }
    else if (lstParts[5] == "Sep")
    {
        month = 9;
    }
    else if (lstParts[5] == "Oct")
    {
        month = 10;
    }
    else if (lstParts[5] == "Nov")
    {
        month = 11;
    }
    else if (lstParts[5] == "Dec")
    {
        month = 12;
    }
    QString dt = QString("%1-%2-%3 %4").arg(QDate::currentDate().year()).arg(month).arg(lstParts[6]).arg(lstParts[7]);
    oneInfo.strMdfyTime = QDateTime::fromString(dt, "yyyy-M-dd hh:mm").toString("yyyyMMddhhmmss");

}

void CurlFtp::setFtpTransferMode(int mode)
{
    ftp_transfermode = mode;

}

void CurlFtp::setFtpConnectMode(int mode)
{
    ftp_connectmode = mode;
    // 主动模式

}

void CurlFtp::setSubDirFlag(bool flag)
{
    m_subDirFlag = flag;
}

void CurlFtp::setSubDirTemplateId(const QString &templateId)
{
    m_strSubDirTemplate = templateId;
}

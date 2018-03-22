﻿#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QTextCodec>
#include <QCoreApplication>
#include <iostream>
#include <QFileInfo>
#include <sstream>
#include <iomanip>
#include <QDateTime>

#include <QThread>
#include <QDir>
#include <QDateTime>
#include <QLockFile>

#include "ctkLog.h"
#include <string>
//#include <log4cplus/loggingmacros.h>
//#include <log4cplus/configurator.h>
//#include <log4cplus/helpers/loglog.h>
//#include <log4cplus/helpers/stringhelper.h>
//#include <log4cplus/helpers/fileinfo.h>
//#include <log4cplus/loggingmacros.h>
//
//
//using namespace std;
//using namespace log4cplus;
//using namespace log4cplus::helpers;

//#include <log4cpp/Category.hh>
//#include <log4cpp/PropertyConfigurator.hh>

//#include "QsLogThread.h"
QLogger *QLogger::s_instance = NULL;

#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

static QMutex sLocker;	// 单例模式锁

QList<OutPutFunPtr> QLogger::m_printer;	// 日志输出函数指针

QLogger::QLogger( qint64 size /* = 10M */, qint64 num /* = 10 */ )
{
    m_iLogNum = num;
    m_iLogSize = size;
    m_bEnableLog = false;	// 默认不启用日志输出

    // 将FilePath中的前缀与后缀截断掉，得到appName

    m_strAppName = qApp->applicationFilePath ();
    m_strAppName.replace("\\", "/");
    int begin = m_strAppName.lastIndexOf("/")+1;
    int end = m_strAppName.lastIndexOf(".");
    m_strAppName = m_strAppName.mid(begin, end-begin);

    //生成存放log的目录路径
    m_strLogPath = QString("%1/log/").arg(QString(qApp->applicationDirPath()));
    QDir sLogDir(m_strLogPath);
    if (!sLogDir.exists())
    {
        sLogDir.mkpath(m_strLogPath);
    }
    m_strLogPath = QString("%1%2.log").arg(m_strLogPath).arg(m_strAppName);

    //QString lockFile = m_strLogPath + ".lock";
    //if (!QFile::exists(lockFile))
    //{
    //    QFile oFile(lockFile);
    //    oFile.open(QIODevice::WriteOnly);
    //    oFile.close();
    //}

    //设置log的路径（sLogPath）与、单个log文件的大小（size）及log文件的个数（num）
    //QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(
    //          sLogPath, QsLogging::EnableLogRotation, QsLogging::MaxSizeBytes(size), QsLogging::MaxOldLogCount(num)));

    //logger.addDestination(fileDestination);

    //std::string initFileName = QString("%1/config/%2.logproperties").arg(qApp->applicationDirPath()).arg(m_strAppName).toStdString();
    //log4cpp::PropertyConfigurator::configure(initFileName);

    //log4cpp::Category& root = log4cpp::Category::getRoot();

    //log4cpp::Category& sub1 =
    //    log4cpp::Category::getInstance(std::string("sub1"));
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(initFileName.c_str()));

}

//MeteoGisLogger::MeteoGisLogger() : logger(QsLogging::Logger::instance())
//{
//
//}

QLogger::~QLogger()
{
    // QsLogging::Logger::destroyInstance();
}

void QLogger::printDebug( const QString& msg, const char* file /*= NULL*/, const char* function /*= NULL*/, int line /*= -1 */ )
{
    if (!m_bEnableLog)
    {
        return;
    }
    print("Debug", msg, file, function, line);

    //QString str = print2(msg, file, function, line);

    //log4cpp::Category& sub1 =
    //   log4cpp::Category::getInstance(std::string("sub1"));
    //sub1.debug(str.toStdString());
    //LOG4CPLUS_DEBUG(Logger::getRoot(), str.toStdString().c_str());
}

void QLogger::printInfo( const QString& msg, const char* file /*= NULL*/, const char* function /*= NULL*/, int line /*= -1 */ )
{
    if (!m_bEnableLog)
    {
        return;
    }
    print("Info", msg, file, function, line);

    //QString str = print2(msg, file, function, line);

    //log4cpp::Category& sub1 =
    //   log4cpp::Category::getInstance(std::string("sub1"));
    //sub1.info(str.toStdString());
    //LOG4CPLUS_INFO(Logger::getRoot(), str.toStdString().c_str());
}

void QLogger::printError( const QString& msg, const char* file /*= NULL*/, const char* function /*= NULL*/, int line /*= -1 */ )
{
    if (!m_bEnableLog)
    {
        return;
    }
    print("Error", msg, file, function, line);
    //QString str = print2(msg, file, function, line);

    //log4cpp::Category& sub1 =
    //   log4cpp::Category::getInstance(std::string("sub1"));
    //sub1.error(str.toStdString());
    //LOG4CPLUS_ERROR(Logger::getRoot(), str.toStdString().c_str());
}

void QLogger::print(const QString &level, const QString& msg, const char* file /*= NULL*/, const char* function /*= NULL*/, int line /*= -1 */ )
{
    //QMutexLocker lockGuard(&m_oLocker)
    QMutexLocker locker(&m_oLocker);
    QString lockFile = m_strLogPath + ".lock";
    QLockFile fileLocker(lockFile);
    bool flag = fileLocker.lock();
    QString m;

    if ( !file )
    {
        m =  QString("[%1] %2").arg(level).arg(msg);
    }
    else if ( !function )
    {
        m = QString( "%1: [%2] %3" ).arg( file ).arg(level).arg( msg );
    }
    else if ( line == -1 )
    {
        m = QString( "%1: (%2) [%3] %4" ).arg( file ).arg( function ).arg(level).arg( msg );
    }
    else
    {
#ifndef _MSC_VER
        m = QString( "%1: %2: (%3) [%4] %5" ).arg( file ).arg( line ).arg( function ).arg(level).arg( msg );
#else
        m = QString( "%1(%2) : (%3) [%4][%5] %6" ).arg( file ).arg( line ).arg( function ).arg(level).arg((int)QThread::currentThreadId()).arg( msg );
#endif
    }
    qDebug( "%s", m.toUtf8().constData() );

    if (getLogFileSize(m_strLogPath) > (m_iLogSize))
    {
        backOldLogFile(m_strLogPath);
    }



    FILE *fp = fopen(m_strLogPath.toLocal8Bit().toStdString().c_str(), "a+");

    if (NULL == fp)
    {
        fileLocker.unlock();
        return;
    }
    m.prepend(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") + " " );
    std::string tmp_strFormat(m.toLocal8Bit().toStdString());
    std::stringstream stream;
    stream << tmp_strFormat << "\n";
    fprintf(fp, stream.str().c_str());
    fclose(fp);

    fileLocker.unlock();
}

QString QLogger::print2(const QString& msg, const char* file /*= NULL*/, const char* function /*= NULL*/, int line /*= -1 */)
{
    QString m;

    if (!file)
    {
        m = msg;
    }
    else if (!function)
    {
        m = QString("%1: %2").arg(file).arg(msg);
    }
    else if (line == -1)
    {
        m = QString("%1: (%2) %3").arg(file).arg(function).arg(msg);
    }
    else
    {
        //#ifndef _MSC_VER
        //m = QString("%1: %2: (%3) [%4] %5").arg(file).arg(line).arg(function).arg(level).arg(msg);
        m = QString("%1(%2) : (%3) %4").arg(file).arg(line).arg(function).arg(msg);
        //#else
        //        m = QString("%1(%2) : (%3) %4").arg(file).arg(line).arg(function).arg(msg);
        //#endif
    }
    qDebug("%s", m.toUtf8().constData());
    return m;
}
//************************************
// Method:    getInstance
// FullName:  MeteoGisLogger::getInstance
// Access:    public static
// Returns:   MeteoGisLogger &
// Qualifier:
//************************************
QLogger  *QLogger::getInstance(bool useFunPrint)
{
    if (NULL == s_instance)
    {
        QMutexLocker lo(&sLocker);
        if (NULL == s_instance)
        {
            s_instance = new QLogger();
        }
    }
    return s_instance;
}

//************************************
// Method:    init
// FullName:  MeteoGisLogger::init
// Access:    public static
// Returns:   void
// Qualifier:
// Parameter: const QString & appName
//************************************
void QLogger::init( qint64 size /* = 10M */, qint64 num /* = 10 */)
{
    if (NULL == s_instance)
    {
        QMutexLocker lo(&sLocker);
        if (NULL == s_instance)
        {
            s_instance = new QLogger(size, num);
        }
    }
}

void QLogger::addOutFunPtr(OutPutFunPtr ptr)
{
    m_printer.append(ptr);
}


void QLogger::backOldLogFile(const QString &in_strFilePath)
{
    QString strFilePath(in_strFilePath);
    QFileInfo fileInfo(strFilePath);

    QDir dir(fileInfo.absoluteDir());
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();

    QRegExp reg("(" + m_strAppName + ")(.log.)(\\d+)");
    int MaxNum = -1;
    //QFileInfoList file2rename;
    foreach (QFileInfo file, list)
    {
        //if (reg.exactMatch(file.fileName()))
        //{
        //	file2rename.append(file);
        //}

        int index = reg.indexIn(file.fileName());
        if (index > -1)
        {
            //file2rename.append(file);
            int num = reg.cap(3).toInt();   // get Num from string
            if (num > MaxNum)
            {
                MaxNum = num;
            }
        }
    }

    if (-1 == MaxNum)
    {
        QFile f(in_strFilePath);
        f.rename(in_strFilePath + ".1");
    }
    else
    {
        if (MaxNum >= m_iLogNum)
        {
            for (int i = m_iLogNum; i <= MaxNum; ++i)
            {
                QFile f(QString("%1.%2").arg(in_strFilePath).arg(i));
                if (!f.remove())
                {
                    std::cerr << "remove failure" << std::endl;
                }
            }


        }

        for (int i = MaxNum; i > 0; i--)
        {
            QFile f(QString("%1.%2").arg(in_strFilePath).arg(i));

            f.rename(QString("%1.%2").arg(in_strFilePath).arg(i+1));
        }

        QFile ff(QString("%1").arg(in_strFilePath));

        ff.rename(QString("%1.1").arg(in_strFilePath));
    }


    //QFile file(in_strFilePath);
    //if (-1 == MaxNum)
    //{
    //	file.rename(fileInfo.absolutePath() + "/" + m_strAppName + ".log.1");
    //}
    //else
    //{
    //	file.rename(fileInfo.absolutePath() + "/" +  m_strAppName +  + ".log." + QString::number(MaxNum + 1));
    //}
}

void QLogger::enableLog(bool in_bEnableLog)
{
    QLogger::getInstance()->m_bEnableLog = in_bEnableLog;
}



#include "LibCurlSFtp.h"
#include <QDataStream>
#include <stdio.h>


SFtp::SFtp()
{

}

SFtp::~SFtp()
{

}


int SFtp::mkdir(const QString &dir)
{
    return 0;
}

int SFtp::put(const QString & localFile, const QString & remoteFile, const QString &suffix /*= QString(".tmp")*/, TransferType type /*= Binary*/)
{
    QString filename, url;
    if (remoteFile.startsWith("/"))
    {
        filename = remoteFile.split("/").last();
        QString dir = remoteFile.mid(0, remoteFile.lastIndexOf("/"));
        url = makeUrl2(dir);
    }
    else
    {
        filename = remoteFile;
        url = makeUrl2("");
    }



    QString cmd = QString("rename %1%2 %1").arg(remoteFile).arg(suffix);
    //QString cmdOrgName = "RNTO " + filename;
    QString strUrl = url + filename + suffix;
    //strUrl += filename + sendsuffix;
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, cmd.toLocal8Bit().toStdString().c_str());
    //headerlist = curl_slist_append(headerlist, cmdOrgName.toLocal8Bit().toStdString().c_str());
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_UPLOAD, 1);
    //////////////////////////////////////////////////////////////////////////
    // 断点续传
    std::FILE *pfile = fopen(localFile.toLocal8Bit().toStdString().c_str(), "rb");
    if (pfile == NULL)
    {
        QSLOG_ERROR(QString("open file %1 failure").arg(localFile));
        return -1;
    }
    QSharedPointer<std::FILE> autoclose(pfile, fclose);
    double size = getFileSize(remoteFile);
    fseek(pfile,0 , SEEK_END);
    long fileLen = ftell(pfile);
    if (size != double(-1.0) )
    {
        if ((long)size != fileLen)
        {
            if (0 != remove(remoteFile))
            {
                QSLOG_ERROR(errorString());
            }
        }
        else
        {
            // 文件已经存在
            return m_iRetCode;
        }
    }
    fseek(pfile, 0, SEEK_SET);


    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_APPEND, 0);
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, strUrl.toLocal8Bit().toStdString().c_str());
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_READDATA, pfile);
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_READFUNCTION, ReadFromFile);
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_POSTQUOTE, headerlist);
    m_iRetCode = curl_easy_perform(m_pCurlHandler);
    curl_slist_free_all(headerlist);
    return m_iRetCode;
}

int SFtp::put(const QByteArray & data, const QString & file, TransferType type /*= Binary*/)
{
    return 0;
}

int SFtp::rename(const QString &oldname, const QString &newname)
{
    return 0;
}


int SFtp::remove(const QString &file)
{
    QString filename, url;
    if (file.startsWith("/"))
    {
        filename = file.split("/").last();
        QString dir = file.mid(0, file.lastIndexOf("/"));
        url = makeUrl2(dir);
    }
    else
    {
        filename = file;
        url = makeUrl2("");
    }

    QString szCmd = QString("rm %1").arg(file);
    //m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url);
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, szCmd.toLocal8Bit().toStdString().c_str());
    m_iRetCode = curl_easy_setopt(m_pThridCurlHandler, CURLOPT_POSTQUOTE, headerlist);
    m_iRetCode = curl_easy_setopt(m_pThridCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
    prepare();
    m_iRetCode = curl_easy_perform(m_pThridCurlHandler);
    curl_slist_free_all(headerlist);
    return m_iRetCode;
}


int SFtp::rmdir(const QString &dir)
{
    QString dirname, url;
    //if (dir.startsWith("/"))
    //{
    //	dirname = dir.split("/").last();
    //	QString dir = dir.mid(0, dir.lastIndexOf("/"));
    //	url = makeUrl(dir);
    //}
    //else
    {
        dirname = dir;
        url = makeUrl2("");
    }
    QString szCmd = QString("rmdir %1").arg(dir);
    //m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url);
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, szCmd.toLocal8Bit().toStdString().c_str());
    m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_POSTQUOTE, headerlist);
    m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
    prepare();
    m_iRetCode = curl_easy_perform(m_pSecCurlHandler);
    curl_slist_free_all(headerlist);
    return m_iRetCode;
}


QString SFtp::makeUrl(const QString &host, quint16 port)
{
    m_urlBody.append(QString("sftp://%1:%2").arg(host).arg(port));
    return makeUrl2("");
}
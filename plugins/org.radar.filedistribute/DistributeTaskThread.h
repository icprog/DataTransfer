﻿#ifndef DistributeTaskThread_H_
#define DistributeTaskThread_H_

#include <QRunnable>
#include <QObject>
#include "curlftp.h"
#include "FileDistributeThread.h"

class SignalSender : public QObject
{
    Q_OBJECT
public:
    virtual ~SignalSender() {}

    void emittaskFinished(int id);

signals:
    void taskFinished(int);
};

/**
  * @class <DistributeTaskThread>
  * @brief  一个文件对应一个线程
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
 */
class DistributeTaskThread : public QRunnable
{
public:
    DistributeTaskThread(FileDistributeThread *pManager, const TransFileInfo &tInfo);
    virtual ~DistributeTaskThread();

    virtual void run();

    int getCurlAddr();
    TransFileInfo m_fileInfo;

    //SignalSender m_signalObj;

protected:

    /**
    * @brief  从共享目录收集数据
    * @param  MemoryData & mData：文件内容
    * @return bool：成功返回true，否则false
    */
    bool collectFromDir(MemoryData &mData);

    bool sendToDir(MemoryData &mData,  KeyIv &keyiv, SendAcctInfo &userInfo);

    bool sendToFtp(MemoryData &mData, KeyIv &keyiv, SendAcctInfo &userInfo);

    /**
     * @brief  发送文件到共享目录
     * @param  const char * fullPath：源文件全路径
     * @param  KeyIv & keyiv：加密压缩信息
     * @param  SendAcctInfo & userInfo：分发用户信息
     * @return bool：成功返回true
     */
    bool sendToDir(const char *fullPath, KeyIv &keyiv, SendAcctInfo &userInfo);
    bool sendToFtp(const char *fullPath, KeyIv &keyiv, SendAcctInfo &userInfo);

    /**
    * @brief  将内存数据写入文件
    * @param  const QString & fileName：文件全路径
    * @param  const MemoryData & mData：内存数据
    * @return bool：成功返回true，否则返回false
    */
    bool writeFileFromBuffer(const QString &fileName, const MemoryData &mData, bool bConput);

    bool writeKeyIv(const QString &fileName, KeyIv &keyiv);

    //bool CrypCurtFile(bool bComp, bool bCrypt, int &nIndex);

    bool CrypCurtFile(const QString &srcFilePath, bool bComp, bool bCrypt, int &nIndex, QString &dstFilePath);

    bool loginToSendRemote(SendAcctInfo &userInfo);

    QString getHostFromFileName(const QString& r_strFullPathFileName);

    QString getCurrentPath(const SendAcctInfo &userInfo);

    QString getCollectSubPath();

private:
    QSharedPointer<CurlFtp>	m_pFtp;
    FileDistributeThread *m_pManager;
    IFileCrypt	*m_pFileCrypt;
    MemoryData	m_mData[4];		///< 0-不加密不压缩，1-只加密，2-只压缩，3-加密压缩
    KeyIv		m_keyiv[4];		///< 加密使用的key和iv

    //static QString s_suffix[4];
};

#endif
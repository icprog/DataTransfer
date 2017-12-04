/**
  * @file   ftpCollector.h
  * @brief  ftp��ʽ�ռ�����.
  *
  *  ��������ϸ˵����
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:24
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef ftpCollector_H_
#define ftpCollector_H_

#include "CollectorBase.h"
#include "CltDispatch.h"
//#include "MyFtp.h"
#include "curlftp.h"

class CurlFtp;

class FtpCollector : public CollectorBase
{
    Q_OBJECT
public:
    FtpCollector(CollectManager *pManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);
    ~FtpCollector();

    virtual int start();
    virtual int reStart();

public slots:
    virtual int stop();
public:
    virtual void getNewFiles();

    virtual void getNewFiles(const CollectTask &in_oTask);

    virtual bool testCollection();

    virtual void taskDone(bool bFlag, const FileInfo &file);

    // ��ͬ����true����ͬ����false
    bool compareWithDest(CurlFtp &oCurlFtp, const FileInfo &fi, TransTask &tTask);

private slots:
    void ftpDone();

private:
    QSharedPointer<QObject> m_pTimerObj;
    QSharedPointer<CltDispatch> m_oProcess;

    //void getNewFiles(const QString &strDirPath, FileInfoList &fileList);

    //QSharedPointer<MyFtp> m_pFtp;
    QSharedPointer<CurlFtp> m_pCftp;	///< libcurl ftp ����

    FileInfoList m_fileList;
};

#endif
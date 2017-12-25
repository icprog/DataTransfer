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


class CurlFtp;

class DLL_EXPORT_CLASS_DECL FtpCollector : public CollectorBase
{
    Q_OBJECT
public:
    FtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);
    ~FtpCollector();

    virtual int start();
    virtual int reStart();

public slots:
    virtual int stop();

    // ���յ��ӽ����˳���Ϣ
    void stoprcv(int exitcode, QProcess::ExitStatus status);
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

    QSharedPointer<CurlFtp> m_pCftp;	///< libcurl ftp ����

    FileInfoList m_fileList;

    bool m_bChildProcessRunning;	// �ӽ������б�ʶ
};

#endif
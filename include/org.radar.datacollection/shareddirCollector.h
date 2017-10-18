/**
  * @file   shareddirCollector.h
  * @brief  ����(����)Ŀ¼�ռ���.
  *
  *  ��������ϸ˵����
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef shareddirCollector_H_
#define shareddirCollector_H_


#include "filewatcher.h"
#include "CollectorBase.h"
#include "commontypedef.h"
#ifdef WITH_QT5
#include <QtConcurrent/QtConcurrent>
#else
#include <QFuture>
#include <QtCore>
using namespace QtConcurrent;
#endif
#include <QList>

class CltDispatch;
class IFileWatcher;
class CurlFtp;

class SharedDirCollector : public CollectorBase
{
    Q_OBJECT
public:
    /**
     * @brief  Ĭ�Ϲ��캯��
     */
    SharedDirCollector(CollectManager *pManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);

    /**
     * @brief  Ĭ����������
     */
    virtual ~SharedDirCollector();

    /**
     * @brief  ��������Ŀ¼�ռ�
     * @return int�������ɹ�����0��ʧ�ܷ���-1
     */
    virtual int start();

    virtual int reStart();

public slots:
    virtual int stop();
    void onCheckDir(QSet<QString>);
    //void fileCreated(const QString &strFilePath);
public:
    /**
     * @brief  ��д�����麯��������ʱ���ã��������ļ�
     * @return void����
     */
    virtual void getNewFiles();

    virtual void getAllFiles(FileInfoList &fileList, QString strPath);

    virtual bool testCollection();

    void syncTransfer();
    void getSynclessFiles(QString strDir, bool bSubdir);

    // ��ͬ����true����ͬ����false
    bool compareWithDest(CurlFtp &oCurlFtp, const QFileInfo &fi, TransTask &tTask);

    /**
     * @brief  ���ݴ���������ļ�
     * @param  QString strDir��Ŀ¼·��
     * @param  FileInfoList & fileList�����ļ��б�
     * @param  bool bRoot���Ƿ�Ϊ��Ŀ¼
     * @return void����
     */
    void getNewFiles(QString strDir, FileInfoList &fileList, bool bRoot=false);

    /**
     * @brief  ��ȡ�޸�ʱ��仯�˵�����Ŀ¼
     * @param  QString strDir����Ŀ¼
     * @param  QStringList & lstDir����Ŀ¼
     * @return void����
     */
    void getNewDirs(QString strDir, QStringList &lstDir);

    /**
     * @brief  ����lstDir���������ļ�
     * @param  const QStringList & lstDir��Ŀ¼�б�
     * @param  FileInfoList & fileList���ļ��б�
     * @return void����
     */
    void getNewFilesFromDir(const QStringList &lstDir, FileInfoList &fileList);

    void getNewDirs2(QString strDir, QStringList &lstDir);
    void getNewFilesFromDir2(const QStringList &lstDir, FileInfoList &fileList);

    /**
     * @brief  ���ݴ���ϵͳ�ռ�����
     * @return void����
     */
    void GetNewFilesToTransfer();

    /**
     * @brief  ͨ�������ռ�
     * @return void����
     */
    void CollectNewFiles();

    /**
     * @brief  ���ӹ���Ŀ¼
     * @param  const QString & strURL������Ŀ¼·��
     * @param  const QString & strUser���û���
     * @param  const QString & strPwd������
     * @return bool�����ӳɹ�����true�����򷵻�false
     */
    //bool connectToWinRemote(QString strURL,const QString& strUser,const QString& strPwd);

    virtual void taskDone(bool bFlag, const FileInfo &file);

    bool m_bConnected;

    bool ProcessFile(const QString &strFileFullPath);

private:
    QSharedPointer<QObject> m_pTimerObject;
    QSharedPointer<CltDispatch> m_oProcessor;

    /// ʵʱ��ض��󣬹���Ŀ¼��ʽ����
    QSharedPointer<IFileWatcher> m_pWatcher;

    void Init();

    QMutex m_oMutex;
    QMutex m_threadNumMutex;

    //static void makeVtbFun(SharedDirCollector *pParent, FileInfo fileinfo);

    QList<QFuture<void> > m_threadFun;	///< ��¼��ǰ���еı�׼���߳�

    //QSharedPointer<FileWatcher> m_pFileWatcher;		///< ʵʱ���
};

#endif

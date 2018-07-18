#ifndef _LIBCURLFTPBASE_H_
#define _LIBCURLFTPBASE_H_

/************************************************************************/
/*                           ��˵������װlibcurl                         */
/************************************************************************/

#include <QString>
#include <QIODevice>
#include <QDateTime>

#include <curl/curl.h>
#include "../macro.h"
#include "curlftp.h"


// FTP����ģʽ
enum TransferMode
{
    Active = 0,
    Passive = 1
};

enum TransferType
{
    Binary = 0,
    Ascii = 1
};

enum FileType
{
    FTP_FILE = 0,
    FTP_DIR = 1
};

class DLL_EXPORT_CLASS_DECL CFileInfo
{
public:
    QString name;		// �ļ���
    QString path;		// ȫ·��
    FileType type;		// �ļ�����
    QDateTime time;		// ����޸�ʱ��
    qint64 size;		// �ļ���С����λ���ֽڣ�
};

class DLL_EXPORT_CLASS_DECL FtpBase
{
public:

    FtpBase();

    virtual ~FtpBase();

    // ���뵽�ƶ���Ŀ¼
    int cd(const QString &dir);

    // �رյ�ǰftp����
    int close();

    // ���ӵ�ftp������
    int connectToHost(const QString &host, quint16 port = 21);

    int connectToHost(const QString &host, quint16 port /*= 21*/, const QString &user /*= QString()*/, const QString &pwd /*= QString()*/, int timeout  = 30);

    // ������һ�δ�����
    QString errorString();

    // �Ƿ�����curl�ڲ���ӡ��Ϣ
    int enableDebugLevel(bool flag = true);

    // ��ftp�����ļ�
    virtual int get(const QString &file, const QString &dir, TransferType type = Binary) = 0;

    // ��ftp�����ļ�(֧���ļ�����أ�ֱ�����ڴ�)
    virtual int get(const QString &file, QIODevice *dev, TransferType type = Binary) = 0;

    // ��ȡftp�ļ���С
    double getFileSize(const QString &file);

    // ��ȡ�ļ�Ŀ¼�б�
    QList<CFileInfo> list(const QString &dir = QString());

    // ��ȡ�ļ�Ŀ¼�б��ݹ������Ŀ¼��
    QList<CFileInfo> listRecursion(const QString &dir = QString());

    // ��½��ftp������(30��)
    int login(const QString &user = QString(), const QString &password = QString(), int timeout = 30);

    // ����Ŀ¼
    virtual int mkdir(const QString &dir) = 0;

    // �ϴ���Ŀ���ļ�
    virtual int	put(const QString & localFile, const QString & remoteFile, const QString &suffix = QString(".tmp"), TransferType type = Binary) = 0;

    // �ϴ���Ŀ���ļ�
    virtual int	put(const QByteArray & data, const QString & file, TransferType type = Binary) = 0;

    // ������
    virtual int rename(const QString &oldname, const QString &newname) = 0;

    // ɾ���ļ�
    virtual int remove(const QString &file) = 0;

    // ɾ��Ŀ¼
    virtual int rmdir(const QString &dir) = 0;

    // ����ftp����ģʽ����������
    int setTransferMode(TransferMode mode);

    // ��������ip���˿���װurl
    virtual QString makeUrl(const QString &host, quint16 port) = 0;

    // ����CD�����������Ŀ¼
    QString makeUrl2(const QString &dir);

    // ��ȡ��ǰ���ڵ����Ŀ¼
    QString getCurrentUrl();

    // easy_perform׼������
    void prepare();


    // ����ftp LIST����ص��ַ���
    void parseMlsdInfo(const QString &rootPath, const QString &info, QList<CFileInfo> &fileList);

    // ����4�ηָ�����ftp
    void process4Ftp(CListInfo &oneInfo, const QStringList &lstParts);

    // ����9�ηָ�����ftp
    void process9Ftp(CListInfo &oneInfo, const QStringList &lstParts);

protected:
    // �ڲ�ά��curl���
    CURL *m_pCurlHandler;

    // ����curl�������Ҫ���ڻ�ȡ�ļ����Ե�
    CURL *m_pSecCurlHandler;

    // ����curl�������Ҫ���ڻ�ȡ�ļ����Ե�
    CURL *m_pThridCurlHandler;

    MemoryData listInfo;

    // ������
    CURLcode m_iRetCode;

    // ���ʵ�ftp������ip
    QString m_strHost;

    // ���ʵ�ftp�������˿�
    qint16 m_iPort;

    QList<QString> m_urlBody;
};

#endif
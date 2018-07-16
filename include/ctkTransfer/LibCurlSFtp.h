#ifndef _LIBCURLSFTP_H_
#define _LIBCURLSFTP_H_

/************************************************************************/
/*                           ��˵������װlibcurl                         */
/************************************************************************/

#include <QString>
#include <QIODevice>
#include <QDateTime>

#include <curl/curl.h>
#include "../macro.h"
#include "curlftp.h"
#include "LibCurlFtp.h"


	//// FTP����ģʽ
 //   enum TransferMode
 //   {
 //       Active = 0,
 //       Passive = 1
 //   };

 //   enum TransferType
 //   {
 //       Binary = 0,
 //       Ascii = 1
 //   };

 //   enum FileType
 //   {
 //       FTP_FILE = 0,
 //       FTP_DIR = 1
 //   };

 //   class DLL_EXPORT_CLASS_DECL CFileInfo
 //   {
 //   public:
 //       QString name;		// �ļ���
 //       QString path;		// ȫ·��
 //       FileType type;		// �ļ�����
 //       QDateTime time;		// ����޸�ʱ��
 //       qint64 size;		// �ļ���С����λ���ֽڣ�
 //   };

	//bool compareCFileInfo(const CFileInfo &f1, const CFileInfo &f2)
	//{
	//	return f1.path.length() > f2.path.length();
	//	
	//}


    class DLL_EXPORT_CLASS_DECL SFtp
    {
    public:

		SFtp();

		virtual ~SFtp();

        // ���뵽�ƶ���Ŀ¼
        int cd(const QString &dir);

        // �رյ�ǰftp����
        int close();

        // ���ӵ�ftp������
        int connectToHost(const QString &host, quint16 port = 22);

        // ������һ�δ�����
        QString errorString();

        // �Ƿ�����curl�ڲ���ӡ��Ϣ
        int enableDebugLevel(bool flag = true);

        // ��ftp�����ļ�
		int get(const QString &file, const QString &dir, TransferType type = Binary);

        // ��ftp�����ļ�(֧���ļ�����أ�ֱ�����ڴ�)
		int get(const QString &file, QIODevice *dev, TransferType type = Binary);

        // ��ȡftp�ļ���С
        double getFileSize(const QString &file);

        // ��ȡ�ļ�Ŀ¼�б�
        QList<CFileInfo> list(const QString &dir = QString());

        // ��ȡ�ļ�Ŀ¼�б��ݹ������Ŀ¼��
		QList<CFileInfo> listRecursion(const QString &dir = QString());

        // ��½��ftp������(30��)
        int login(const QString &user = QString(), const QString &password = QString(), int timeout = 30);

        // ����Ŀ¼
        int mkdir(const QString &dir);

        // �ϴ���Ŀ���ļ�
		int	put(const QString & localFile, const QString & remoteFile, const QString &suffix = QString(".tmp"), TransferType type = Binary);

        // �ϴ���Ŀ���ļ�
		int	put(const QByteArray & data, const QString & file, TransferType type = Binary);

        // ������
        int rename(const QString &oldname, const QString &newname);

        // ɾ���ļ�
        int remove(const QString &file);

        // ɾ��Ŀ¼
        int rmdir(const QString &dir);

        // ����ftp����ģʽ����������
		int setTransferMode(TransferMode mode);

    private:
        // ��������ip���˿���װurl
        QString makeUrl(const QString &host, quint16 port);

        // ����CD�����������Ŀ¼
        QString makeUrl(const QString &dir);

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

    private:
        // �ڲ�ά��curl���
        CURL *m_pCurlHandler;

        // ����curl�������Ҫ���ڻ�ȡ�ļ����Ե�
        CURL *m_pSecCurlHandler;

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
#ifndef _LIBCURLFTP_H_
#define _LIBCURLFTP_H_

/************************************************************************/
/*                           ��˵������װlibcurl                         */
/************************************************************************/

#include <QString>
#include <QIODevice>
#include <QDateTime>

#include <curl/curl.h>
#include "../macro.h"
#include "curlftp.h"

namespace FTP
{
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
		FILE = 0,
		DIR = 1
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

	class DLL_EXPORT_CLASS_DECL CFtp
	{
	public:

		CFtp();

		virtual ~CFtp();

		// ���뵽�ƶ���Ŀ¼
		int cd(const QString &dir);

		// �رյ�ǰftp����
		int close();

		// ���ӵ�ftp������
		int connectToHost(const QString &host, quint16 port = 21);

		// ������һ�δ�����
		QString errorString();

		// �Ƿ�����curl�ڲ���ӡ��Ϣ
		int enableDebugLevel(bool flag = true);

		// ��ftp�����ļ�
		int get(const QString &file, const QString &dir, TransferType type = Binary);

		// ��ȡ�ļ�Ŀ¼�б�
		QList<CFileInfo> list(const QString &dir = QString());

		// ��½��ftp������
		int login(const QString &user = QString(), const QString &password = QString());

		// ����Ŀ¼
		int mkdir(const QString &dir);

		// �ϴ���Ŀ���ļ�
		int	put(QIODevice * dev, const QString & file, TransferType type = Binary);

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

		void parseMlsdInfo(const QString &rootPath, const QString &info, QList<CFileInfo> &fileList);
		void process4Ftp(CListInfo &oneInfo, const QStringList &lstParts);
		void process9Ftp(CListInfo &oneInfo, const QStringList &lstParts);

	private:
		// �ڲ�ά��curl���
		CURL *m_pCurlHandler;

		MemoryData listInfo;

		// ������
		CURLcode m_iRetCode;

		// ���ʵ�ftp������ip
		QString m_strHost;

		// ���ʵ�ftp�������˿�
		qint16 m_iPort;

		QList<QString> m_urlBody;
	};
}
#endif
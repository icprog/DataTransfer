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
#include "LibCurlFtpBase.h"

class DLL_EXPORT_CLASS_DECL SFtp : public FtpBase
{
public:

    SFtp();

    virtual ~SFtp();

    // ��ftp�����ļ�
    virtual int get(const QString &sourceFile, const QString &downloadFile, TransferType type = Binary);

    // ��ftp�����ļ�(֧���ļ�����أ�ֱ�����ڴ�)
    virtual int get(const QString &file, QIODevice *dev, TransferType type = Binary);


    // ����Ŀ¼
    virtual int mkdir(const QString &dir);

    // �ϴ���Ŀ���ļ�
    virtual int	put(const QString & localFile, const QString & remoteFile, const QString &suffix = QString(".tmp"), TransferType type = Binary);

    // �ϴ���Ŀ���ļ�
    virtual int	put(const QByteArray & data, const QString & file, TransferType type = Binary);

    // ������
    virtual int rename(const QString &oldname, const QString &newname);

    // ɾ���ļ�
    virtual int remove(const QString &file);

    // ɾ��Ŀ¼
    virtual int rmdir(const QString &dir);

    QString makeUrl(const QString &host, quint16 port);
};

#endif
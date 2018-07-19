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
#include "LibCurlFtpBase.h"

class DLL_EXPORT_CLASS_DECL CFtp : public FtpBase
{
public:

    CFtp();

    virtual ~CFtp();

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

    // ��������ip���˿���װurl
    virtual QString makeUrl(const QString &host, quint16 port);
};

#endif
#include "FileCrypt.h"
#include "ctkLog.h"
#include "zlib.h"
#include <QFileInfo>
#include <QDataStream>

#pragma warning(disable:4996)

FileCryptImp::FileCryptImp()
    : m_pIcrypt(NULL)
{
    m_regMatch.setPattern(".[cu][eu]");
}

FileCryptImp::~FileCryptImp()
{

}

bool FileCryptImp::CompCryptFile(const QByteArray& raw, QByteArray& result, KeyIv& keyiv, const bool& bcomp, const bool& bencrypt)
{
    result = raw;

    // ��ѹ��
    if (bcomp)
    {
        result = qCompress(raw);
    }

    // �ټ���
    if (bencrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }
        QByteArray baCipher;
        if (m_pIcrypt->encrypt(result, keyiv.key, keyiv.iv, baCipher))
        {
            result = baCipher;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool FileCryptImp::CompCryptFile(const QString &srcFilePath, const QString &dstFilePath, KeyIv &key)
{
    QByteArray baSrc = srcFilePath.toLocal8Bit();
    const char *srcPath = baSrc.data();
    QByteArray baDst = dstFilePath.toLocal8Bit();
    const char *dstPath = baDst.data();
    QByteArray baTmp;
    const char *tmpPath;

    if (key.bComp && key.bCrypt)
    {
        baTmp = (dstFilePath + ".tmp").toLocal8Bit();
        tmpPath = baTmp.data();
    }
    else if (key.bComp)
    {
        tmpPath = dstPath;
    }
    else if (key.bCrypt)
    {
        tmpPath = srcPath;
    }

    // ��ѹ��
    if (key.bComp)
    {
        if (Compress(srcPath, tmpPath) != 0)
        {
            QSLOG_ERROR(QStringLiteral("ѹ���ļ�ʧ�ܣ�"));
            return false;
        }

        //srcPath = tmpPath;
    }

    // �ټ���
    if (key.bCrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        if (!m_pIcrypt->encryptFile(tmpPath, key.key, key.iv, dstPath))
        {
            QSLOG_ERROR(QStringLiteral("�����ļ�ʧ�ܣ�"));
            return false;
        }
    }

    return true;
}

bool FileCryptImp::UCompCryptFile(const QByteArray& raw, QByteArray& result, const KeyIv& keyiv, const bool& bcomp, const bool& bencrypt)
{
    result.clear();

    // �Ƚ���
    if (bencrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        if (!m_pIcrypt->decrypt(raw, keyiv.key, keyiv.iv, result))
        {
            return false;
        }
        QSLOG_DEBUG(QStringLiteral("���ܳɹ�"));
    }

    // �ٽ�ѹ
    if (bcomp)
    {
        result = qUncompress(result);
        QSLOG_DEBUG(QStringLiteral("��ѹ�ɹ�"));
    }

    return true;
}

// bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QByteArray &result)
// {
// 	QString strKeyFile = strFileFullPath + ".key";
// 	QFileInfo fileInfo(strFileFullPath);
// 	if (!fileInfo.isFile())
// 	{
// 		return false;
// 	}
// 	QString strFileName = fileInfo.fileName();
//
// // 	QString strLast = strFileName.right(3);
// // 	if (!m_regMatch.exactMatch(strLast))
// // 	{
// // 		return false;
// // 	}
//
// // 	bool bComp = (strLast.mid(1,1) == "u") ? false : true;
// // 	bool bCrypt = (strLast.right(1) == "u") ? false : true;
//
// 	KeyIv keyiv;
// 	//if (bCrypt)
// 	{
// 		// ��ȡ�����ļ�
// 		strFileName = strFileName.left(strFileName.length() - 3);
// 		//QString strKeyFullPath = fileInfo.path() + "/keys/" +strFileName + ".key";
// 		QString strKeyFullPath = fileInfo.path() +strFileName + ".key";
// 		QFileInfo fileInfo2(strKeyFullPath);
// 		if (!fileInfo2.isFile())
// 		{
// 			return false;
// 		}
// 		QFile fileKey(strKeyFullPath);
// 		if (!fileKey.open(QFile::ReadOnly))
// 		{
// 			return false;
// 		}
// // 		int a = fileKey.size();
// // 		int b = sizeof(KeyIv);
// // 		fileKey.read((char *)&keyiv, a);
// 		QDataStream in(&fileKey);
// 		in >> keyiv.key;
// 		in >> keyiv.iv;
// 		fileKey.close();
// 	}
//
// 	QFile fileData(strFileFullPath);
// 	if (!fileData.open(QFile::ReadOnly))
// 	{
// 		return false;
// 	}
// 	QByteArray baOrg = fileData.readAll();
// 	result.clear();
// 	return UCompCryptFile(baOrg, result, keyiv, bComp, bCrypt);
// }

bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QByteArray &result)
{
    QSLOG_DEBUG(QStringLiteral("��ʼ���ܽ�ѹ[%1]").arg(strFileFullPath));

    QFile fileInfo(strFileFullPath);
    if (!fileInfo.exists())
    {
        return false;
    }

    QString strKeyFullPath = strFileFullPath + ".key";
    if (!fileInfo.exists(strKeyFullPath))
    {
        return false;
    }

    // ��ȡ��Կ
    KeyIv keyiv;
    QFile fileKey(strKeyFullPath);
    if (!fileKey.open(QFile::ReadOnly))
    {
        return false;
    }
    QDataStream in(&fileKey);
    in >> keyiv.bComp;
    in >> keyiv.bCrypt;
    in >> keyiv.key;
    in >> keyiv.iv;
    fileKey.close();

    // ��ȡ����
    QFile fileData(strFileFullPath);
    if (!fileData.open(QFile::ReadOnly))
    {
        return false;
    }
    QByteArray baOrg = fileData.readAll();
    fileData.close();

    result.clear();

    //////////////////////////////////////////////////////////////////////////
    // �Ƚ���
    //if (keyiv.bCrypt)
    //{
    //    getIcrypt();
    //    if (m_pIcrypt == NULL)
    //    {
    //        return false;
    //    }

    //    // QStringתchar*��һ��Ҫ�ֲ���д!!
    //    QString resFullPath = strFileFullPath + ".jm";
    //    QByteArray baTmp1 = strFileFullPath.toLocal8Bit();
    //    QByteArray baTmp2 = resFullPath.toLocal8Bit();
    //    const char *oldPath = baTmp1.data();
    //    const char *newPath = baTmp2.data();
    //    if (!m_pIcrypt->decryptFile(oldPath, keyiv.key, keyiv.iv, newPath))
    //    {
    //        return false;
    //    }

    //    QFile file(resFullPath);
    //    if ( !file.open(QFile::ReadOnly) )
    //    {
    //        return false;
    //    }
    //    result = file.readAll();
    //    file.close();
    //}

    //// �ٽ�ѹ
    //if (keyiv.bComp)
    //{
    //    result = qUncompress(result);
    //}

    //return true;
    //////////////////////////////////////////////////////////////////////////
    return UCompCryptFile(baOrg, result, keyiv, keyiv.bComp, keyiv.bCrypt);
}

bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QString &resFullPath)
{
    QFile fileInfo(strFileFullPath);
    if (!fileInfo.exists())
    {
        return false;
    }

    QString strKeyFullPath = strFileFullPath + ".key";
    if (!fileInfo.exists(strKeyFullPath))
    {
        return false;
    }

    // ��ȡ��Կ
    KeyIv keyiv;
    QFile fileKey(strKeyFullPath);
    if (!fileKey.open(QFile::ReadOnly))
    {
        return false;
    }
    QDataStream in(&fileKey);
    in >> keyiv.bComp;
    in >> keyiv.bCrypt;
    in >> keyiv.key;
    in >> keyiv.iv;
    fileKey.close();

    QByteArray baSrc, baDst, baTmp;
    const char *srcPath, *dstPath, *tmpPath;
    baSrc = strFileFullPath.toLocal8Bit();
    srcPath = baSrc.data();
    baDst = resFullPath.toLocal8Bit();
    dstPath = baDst.data();

    bool bFlag = false;

    if (keyiv.bComp && keyiv.bCrypt)
    {
        baTmp = (resFullPath + ".tmp").toLocal8Bit();
        tmpPath = baTmp.data();
        bFlag = true;
    }
    else if (keyiv.bCrypt)
    {
        tmpPath = dstPath;
    }
    else if (keyiv.bComp)
    {
        tmpPath = srcPath;
    }

    // �Ƚ���
    if (keyiv.bCrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        if (!m_pIcrypt->decryptFile(srcPath, keyiv.key, keyiv.iv, tmpPath))
        {
            QSLOG_ERROR(QStringLiteral("����ʧ�ܣ�"));
            if (bFlag)
            {
                QFile::remove(QString::fromLocal8Bit(baTmp));
            }
            return false;
        }
    }

    // �ٽ�ѹ
    if (keyiv.bComp)
    {
        if (UnCompress(tmpPath, dstPath) != 0)
        {
            QSLOG_ERROR(QStringLiteral("��ѹʧ�ܣ�"));
            if (bFlag)
            {
                QFile::remove(QString::fromLocal8Bit(baTmp));
            }
            return false;
        }
    }

    if (bFlag)
    {
        // ������ôд������ɾ��ʧ��!!
        QFile::remove(QString::fromLocal8Bit(baTmp));
    }
    return true;
}

ICrypt * FileCryptImp::getIcrypt()
{
    if (m_pIcrypt == NULL)
    {
        ICtkPluginManager *pManager = getCtkPluginManager();
        if (pManager != NULL)
        {
            QObject *pObj = pManager->getService("ICrypt");
            if (NULL != pObj)
            {
                m_pIcrypt = qobject_cast<ICrypt *>(pObj);
            }
        }
    }
    return m_pIcrypt;
}

int FileCryptImp::Compress(const char *srcName, const char *dstName)
{
    int res = 0;

    char buff[16384] = {0};
    long len = 0;

    FILE *fp_in = fopen(srcName, "rb");
    if (fp_in == NULL)
    {
        return -1;
    }

    gzFile out = gzopen(dstName, "wb");
    if (out == NULL)
    {
        fclose(fp_in);
        return -1;
    }

    while (true)
    {
        len = fread(buff, 1, sizeof(buff), fp_in);
        if (ferror(fp_in))
        {
            res = -1;
            break;
        }
        if (len == 0)
        {
            res = 0;
            break;
        }

        if (gzwrite(out, buff, len) != len)
        {
            res = -1;
            break;
        }
    }

    gzclose(out);
    fclose(fp_in);

    return res;
}

int FileCryptImp::UnCompress(const char *srcName, const char *dstName)
{
    int res = 0;

    char buff[16384] = {0};
    long len = 0;

    gzFile in = gzopen(srcName, "rb");
    if (in == NULL)
    {
        return -1;
    }

    FILE *fp_out = fopen(dstName, "wb");
    if (fp_out == NULL)
    {
        gzclose(in);
        return -1;
    }

    for (;;)
    {
        len = gzread(in, buff, sizeof(buff));
        if (len < 0)
        {
            res = -1;
            break;
        }

        if (len == 0)
        {
            res = 0;
            break;
        }

        if (fwrite(buff, 1, len, fp_out) != len)
        {
            res = -1;
            break;
        }
    }

    fclose(fp_out);
    gzclose(in);

    return res;
}

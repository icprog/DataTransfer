#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QThread>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QSet>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QObject *parent = 0);
    virtual ~FileWatcher();

signals:
    void checkDir(QSet<QString>);
public slots:
    void dirChanged(QString path);
    void onTimer();

public:
    // ����ļ��м��
    bool addDirWatch(const QString &strRoot, bool bSub=true);
    // ɾ���ļ��м��
    void removeDirWatch(const QString &strRoot, bool bSub=true);

private:
    QStringList listAllDirs(const QString &strPath);
    void addDirToWatch(QString strPath);
    void addDirsToWatch(QStringList strPaths);

    QThread m_oThread;

    QFileSystemWatcher m_fileWatch;
    QSet<QString> m_watchDirs;          // �Ѽ�ص�Ŀ¼

    QSet<QString> m_uncheckDirs;        // ������Ŀ¼

    QTimer m_oTimer;
};

#endif // FILEWATCHER_H

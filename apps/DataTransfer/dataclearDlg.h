#ifndef dataclearDlg_h__
#define dataclearDlg_h__

#include <QDialog>
#include "ui_dataclear.h"
#include "commontypedef.h"

class DataClearDlg : public QDialog
{
    Q_OBJECT
public:
    DataClearDlg(QDialog *parent = NULL);
    DataClearDlg(const QString &taskname, QDialog *parent = NULL);

signals:
    void newTaskCreated(const ClearTask &task);
public slots:
    void openDirOpen();

    // ����½�ɾ������ʱ��ȷ��
    void onApply();

    // �������ʱ��ȷ��
    void onApply2();

    void onItemChanged(const QString &item);

    void onComBoxIndexChanged(int index);

    void onAdded();

    void onDelete();
protected:
    // ��������
    void InitUI();

    // ���Ե���
    void InitUI(const QString &taskName);

    int computeSeconds();
private:
    Ui::Dialog ui;

public:
    ClearTask m_task;
};
#endif // dataclearDlg_h__

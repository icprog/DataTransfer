#ifndef systemset_h__
#define systemset_h__

#include "ui_systemset.h"
#include <QDialog>

class CSystemSetDlg : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief:Ĭ�Ϲ��캯��
     * @param: QDialog * parent
     * @return: ��������ֵ
     */
    CSystemSetDlg(QDialog *parent = NULL);

    /**
     * @brief:Ĭ����������
     * @return: ��������ֵ
     */
    virtual ~CSystemSetDlg();

private:
    Ui::SystemSet ui;
};
#endif // systemset_h__

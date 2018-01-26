#ifndef baseset_h__
#define baseset_h__



#include "ui_baseset.h"

#include <QWidget>

class CBaseSetDlg : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief:Ĭ�Ϲ��캯��
     * @param: QWidget * parent
     * @return: ��������ֵ
     */
    CBaseSetDlg(QWidget *parent = NULL);

    /**
     * @brief:��������
     * @return: ��������ֵ
     */
    virtual ~CBaseSetDlg();

    /**
     * @brief:��ʼ��
     * @return:void ��������ֵ
     */
    void init();

signals:
    // ȷ�ϰ�ť�����沢�ر�
    void wndquit();

    // �˳���ť���ر�
    void wndcancel();

protected slots:
    /**
     * @brief:Ӧ��
     * @return:void ��������ֵ
     */
    void onApply();

    /**
     * @brief:ȷ�ϣ����沢�رգ�
     * @return:void ��������ֵ
     */
    void onOk();

private:
    Ui::baseset ui;
};
#endif // baseset_h__

/**
  * @file   UIInterface.h
  * @brief  UI�ӿ�.
  *
  *  �ṩ��UI�в���˵����������ȹ��ܽӿ�
  * @author ������
  * @par   Copyright (c):
  *          ������.
  *         All Rights Reserved
  * @date   2016:07:04
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef UIInterface_h__
#define UIInterface_h__

#include <QObject>
#include <QAction>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTreeWidgetItem>

class UIInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~UIInterface() {}

public:

    /**
     * @brief  ��ȡ�����ʾ��ָ��
     * @return QWidget *����������ֵ
     */
    virtual QWidget *getWidget() = 0;

    /**
     * @brief  ���˵�����Ӳ˵���
     * @param  const QString m_idenitty �˵�Action����
     * @param  QAction * qAction ��Ӧ�Ķ���
     * @return int����ӳɹ�������0.ʧ�ܷ���-1
     */
    virtual int addAction( const QString m_idenitty, QAction *qAction ) = 0;

    //���鵵�˵�
    virtual void addActToArchive(QAction *qAction ) = 0;

    //�����Ʋ˵�(����)
    virtual void addActToControl(QAction *qAction, bool bToolBar = false ) = 0;

    //���鿴�˵�(��־����)
    virtual void addActToView(QAction *qAction ) = 0;

    //��ͨ�Ų˵������(ͨ�Ų��)
    virtual void addToCommunication(QAction *pAction) = 0;

    /**
     * @brief  ����ĵ�����������Ӵ���
     * @param  QWidget * pWnd
     * @return void����������ֵ
     */
    virtual void addSubWnd(QWidget *pWnd) = 0;

    /**
     * @brief  �Ƴ����ĵ��Ӵ���
     * @param  const QString & title �������
     * @return
     */
    virtual void removeSubWnd(const QString &title) = 0;

    virtual QTreeWidgetItem* AppendItem(const QString& sItem) = 0 ;

    virtual void AppendItem(const QString& sItem, QWidget *widget) = 0;

    virtual void AppendItem(QTreeWidgetItem *pParent, const QString& sItem, QWidget *widget) = 0;

    virtual QTreeWidgetItem* InserItem(int index, const QString& sItem) = 0 ;

    virtual void InsertItem(int index, const QString& sItem, QWidget *widget) = 0;

    virtual void InsertItem(QTreeWidgetItem *pParent, int index, const QString& sItem, QWidget *widget) =0;

    virtual void AddMessageGroup(const QString& sGroupName, const QStringList& slabelList) = 0 ;

    virtual void AddMessageGroupEx(const QString& sGroupName, const QStringList& slabelList, const QStringList& sValueList) = 0 ;

    virtual void AddThreadGroup(const QString& sGroupName, const QStringList& slabelList, const QStringList& sValueList) = 0;

    virtual void showSubWnd(const QString &title) = 0 ;

signals:
    void save(const QString&, const QStringList&);
    void saveThread(const QString&, const QStringList&);
};
#endif // UIInterface_h__

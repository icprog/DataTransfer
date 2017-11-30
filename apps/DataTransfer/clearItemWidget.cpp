#include "clearItemWidget.h"
#include <QDateTime>

ClearItemWidget::ClearItemWidget(const QString &name, QWidget *parent /*= NULL*/) : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui.label->setText(name);
    // �����ޱ߿�
    QPixmap picuture(":/dirclean.png");
    ui.leIcon->setPixmap(picuture);

    // gif����
    m_pMovie = new QMovie(":/working.gif");
    m_pMovie->setSpeed(50);
    ui.le_gif->setMovie(m_pMovie);
    //m_pStatusWnd = new CStatusLabel();
    //QVBoxLayout *pLayout = new QVBoxLayout();
    //ui.le_gif->setLayout(pLayout);
    //pLayout->addWidget(m_pStatusWnd);

    QString strTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString state = QStringLiteral("%1/%2 %3:%4 ����").arg(strTime.mid(4,2)).arg(strTime.mid(6,2))
                    .arg(strTime.mid(8,2)).arg(strTime.mid(10,2));
    ui.label_2->setText(state);
}

void ClearItemWidget::ShowGif(bool bFlag)
{
    if (bFlag)
    {
        ui.le_gif->show();
        m_pMovie->start();
        //m_pStatusWnd->start();
    }
    else
    {
        ui.le_gif->hide();
        m_pMovie->stop();
        //m_pStatusWnd->stop();
    }
}

void ClearItemWidget::SetEnable(bool bFlag)
{
    //if (bFlag)
    //{
    //	m_curIcon = (ICONTYPE)(m_curIcon + 4);
    //}
    //else
    //{
    //	m_curIcon = (ICONTYPE)(m_curIcon - 4);
    //}
    //SetIcon(m_curIcon);
    ui.leIcon->setEnabled(bFlag);
}

void ClearItemWidget::setName(const QString &name)
{
    ui.label->setText(name);
}

void ClearItemWidget::addSuccess(const QDateTime &dt)
{
    QString strTime =  dt.toString("yyyyMMddhhmmss");
    QString state = QStringLiteral("�����%1/%2 %3:%4").arg(strTime.mid(4,2)).arg(strTime.mid(6,2))
                    .arg(strTime.mid(8,2)).arg(strTime.mid(10,2));
    ui.label_2->setText(state);
}

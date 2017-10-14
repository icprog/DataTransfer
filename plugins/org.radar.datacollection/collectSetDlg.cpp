#include "collectSetDlg.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include <QFileDialog>

CollectSetDlg::CollectSetDlg(QWidget *parent/*=NULL*/)
{
    ui.setupUi(this);

    connect(ui.btn_sel, SIGNAL(clicked()), this, SLOT(onBtnSelectPath()));
}

CollectSetDlg::~CollectSetDlg()
{

}

void CollectSetDlg::onBtnSelectPath()
{
    // �򿪵�ǰ���ڵ��ļ���
    QString strPath = QFileDialog::getExistingDirectory(this, "ѡ���ļ���", ui.lineEdit->text(), QFileDialog::ShowDirsOnly);
    if (!strPath.isEmpty())
    {
        ui.lineEdit->setText(strPath);
        ui.lineEdit->setCursorPosition(-1);
    }
}

void CollectSetDlg::loadCollects()
{
    CPathBuilder pd;
    m_strConPath = pd.getConfigPath() + "/CollectSet.xml";
    if (!XmlToCollectSet(m_strConPath.toLocal8Bit().toStdString(), m_lstCollects))
    {
        QSLOG_ERROR("Fail to loadCollects.");
        return;
    }
}

﻿#include "collectSetDlg.h"
#include "userItem.h"
#include "regexpression.h"
#include "pathbuilder.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPalette>
#include <QTimer>
#include <curl/curl.h>
#include <QDebug>

CollectSetDlg::CollectSetDlg(int flag, QDialog *parent /*= NULL*/)
    : m_flag(flag)
    , QDialog(parent)

{
    ui.setupUi(this);
    InitUI();
}

CollectSetDlg::~CollectSetDlg()
{

}

void CollectSetDlg::InitUI()
{
//     ui.tableWidget->setColumnCount(3);
//     ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//     //ui.tableWidget->resizeColumnsToContents();
//     ui.tableWidget->setColumnWidth(0, 100);
//     ui.tableWidget->setColumnWidth(1, 120);
//     ui.tableWidget->resizeRowsToContents();
//     ui.tableWidget->setRowCount(1);

    // 新增
    if (m_flag == 0)
    {
        ui.radFile->setChecked(true);
        onSelFile(true);
        QString dirName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        dirName = QStringLiteral("FILE传输-") + dirName;
        ui.le_DirName->setText(dirName);
        int sRand = qrand() % 60;
        QString timeR = QString::number(sRand) + " * * * * *";
        ui.lineEdit_3->setText(timeR);
        ui.lineEdit_4->setText(".*");
    }

    //ui.checkBox_6->setVisible(false);
    connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(onApply()));
    connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui.btnBrowse, SIGNAL(clicked()), this, SLOT(onBrows()));
    connect(ui.radFile, SIGNAL(toggled(bool)), this, SLOT(onSelFile(bool)));
    //connect(ui.btnSelUser, SIGNAL(clicked()), this, SLOT(selUser()));
    connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(onRegExpTest()));
    connect(ui.btn_test_2, SIGNAL(clicked()), this, SLOT(onWaitForTest()));
    //connect(ui.btn_test_2, SIGNAL(clicked()), this, SLOT(onRemoteColTest()));
    connect(this, SIGNAL(testok()), this, SLOT(onTestOk()));
    connect(this, SIGNAL(testfail()), this, SLOT(onTestFail()));

    // 从数据库读取分发用户
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser) || lstUser.size() == 0)
    {
        return;
    }
    int nIndex = 0;
    for (int i = 0; i < lstUser.size(); ++i)
    {
        nIndex++;
        ui.comboBox_sendUser->addItem(lstUser.at(i).userName);
        //m_iUser[nIndex] = lstUser.at(i).userID;
    }
}

void CollectSetDlg::selUser()
{
//     if (m_flag == 0)
//     {
//         m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg());
//     }
//     else
//     {
//         m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg(m_flag, m_selUser));
//     }
    m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg());

    //if (QDialog::Accepted == m_pSelUserDlg->exec())
    //{
    //    // m_selUser.lstUser.append(m_pSelUserDlg->m_selUser);

    //    QListWidgetItem *pItem = new QListWidgetItem();
    //    ui.listWidget->addItem(pItem);
    //    UserItem *pUser = new UserItem(ui.listWidget, m_pSelUserDlg->m_selUser.user.userID, m_pSelUserDlg->m_selUser.user.userName, m_pSelUserDlg->m_selUser.rltvPath);
    //    connect(pUser, SIGNAL(onDelete(const QString &)), this, SLOT(onSendUserDel(const QString &)));
    //    ui.listWidget->setItemWidget(pItem, pUser);
    //    pItem->setSizeHint(QSize(pUser->rect().width(), pUser->rect().height()));
    //}
}

bool CollectSetDlg::onApply()
{
    CollectTask &cSet = m_task;
    cSet.dirName = ui.le_DirName->text();
    cSet.collectType = (ui.radFile->isChecked()) ?  0 : 1;
    cSet.ftp_transferMode = ui.comboBox_bin->currentIndex();
    cSet.ftp_connectMode = ui.comboBox_passive->currentIndex();
    cSet.rltvPath = ui.le_RelvPath->text();
    cSet.dispatch = ui.lineEdit_3->text();
    cSet.fileTemplate = ui.lineEdit_4->text();
    cSet.subdirFlag = (ui.checkBox_2->isChecked()) ? 1 : 0;
//    cSet.moveFlag = (ui.checkBox_6->isChecked()) ? 1 : 0;
    // cSet.col_timerange = (ui.col_time_range)
    // 0:所有，1:30分钟，2：1小时，3：2小时，4：3小时，5：6小时，6：12小时，7：1天，8：2天，9：3天

    int tmin = 0;
    // 匹配数字部分
    QString strTime = ui.col_time_range->currentText();
    if (strTime != QStringLiteral("所有"))
    {
        QRegExp oReg("([0-9]{1,})(.*)");
        int pos = oReg.indexIn(strTime);
        if (-1 == pos)
        {
            QMessageBox::warning(this, QStringLiteral("收集策略检查"), QStringLiteral("收集时效配置不正确"));
            return false;
        }
        QString strTimeUnit = oReg.cap(2);
        if (QStringLiteral("天") == strTimeUnit)
        {
            tmin = 60 * 24;
        }
        else if (QStringLiteral("小时") == strTimeUnit)
        {
            tmin = 60;
        }
        else if (QStringLiteral("分钟") == strTimeUnit)
        {
            tmin = 1;
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("收集策略检查"), QStringLiteral("收集时效配置不正确"));
            return false;
        }
        tmin = oReg.cap(1).toInt() * tmin;
    }
    else
    {
        tmin = -1;
    }




    /*switch (ui.col_time_range->currentIndex())
    {
    case 0:
    tmin = -1;
    break;
    case 1:
    tmin = 30;
    break;
    case 2:
    tmin = 60;
    break;
    case 3:
    tmin = 60*2;
    break;
    case 4:
    tmin = 60*3;
    break;
    case 5:
    tmin = 60*6;
    break;
    case 6:
    tmin = 60*12;
    break;
    case 7:
    tmin = 60*24;
    break;
    case 8:
    tmin = 60*48;
    break;
    case 9:
    tmin = 60*72;
    break;
    default:
    break;
    }*/
    cSet.col_timerange = tmin;
    cSet.loginUser = ui.lineEdit_5->text();
    cSet.loginPass = ui.lineEdit_6->text();
    cSet.ip = ui.lineEdit_7->text();
    cSet.port = ui.lineEdit_8->text().toInt();
    cSet.recordLatestTime = ui.recordLatestTime->isChecked() ? 1 : 0;

    QMessageBox box(QMessageBox::Critical, QStringLiteral("提示"), "");
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
    if (cSet.dirName.isEmpty())
    {
        box.setText(QStringLiteral("任务名不能为空！"));
        box.exec();
        return false;
    }

    //int size = ui.listWidget->count();
    //if (size <= 0)
    //{
    //    box.setText(QStringLiteral("未配置分发用户！"));
    //    box.exec();
    //    return false;
    //}
    if (m_flag == 0)	// 新增
    {
        cSet.dirID = QUuid::createUuid().toString();
    }
    //else				// 修改
    //{
    //    cSet.dirID = m_selUser.dirID;
    //}
    //m_selUser.lstUser.clear();

    //UserItem *pUserItem = (UserItem *) ui.listWidget->itemWidget(ui.listWidget->item(0));

    m_selUser.lstUser.append(getSendUserInfoFromName(ui.comboBox_sendUser->currentText()));
    //// 临时屏蔽
    //// 更新收集用户表
    m_selUser.taskID = cSet.dirID;
    DataBase::getInstance()->InsertCollectUser(m_selUser);
    // 查询用户信息
    //DataBase::getInstance()->QueryUserInfo(m_selUser);

    accept();
    emit commit(cSet);

    return true;
}

void CollectSetDlg::onCancel()
{
    reject();
    // close();
}

void CollectSetDlg::onBrows()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, QStringLiteral("选择源目录"), ui.le_RelvPath->text());
    if (!path.isEmpty())
    {
        ui.le_RelvPath->setText(path);
        ui.le_RelvPath->setCursorPosition(-1);
    }
}

void CollectSetDlg::onSelFile(bool bFile)
{
    if (bFile)
    {
        ui.label_3->setText(QStringLiteral("收集路径："));

    }
    else
    {
        ui.label_3->setText(QStringLiteral("远程路径："));
    }
    ui.comboBox_bin->setVisible(!bFile);
    ui.comboBox_passive->setVisible(!bFile);

    ui.btnBrowse->setEnabled(bFile);
    ui.lineEdit_5->setEnabled(!bFile);
    ui.lineEdit_6->setEnabled(!bFile);
    ui.lineEdit_7->setEnabled(!bFile);
    ui.lineEdit_8->setEnabled(!bFile);
}

void CollectSetDlg::showTask(const CollectTask &task)
{
    if (m_flag == 0)
    {
        return;
    }

    ui.le_DirName->setText(task.dirName);
    bool bFile = true;
    if (task.collectType == 0)
    {
        ui.radFile->setChecked(true);
        bFile = true;
    }
    else
    {
        ui.radFtp->setChecked(true);
        bFile = false;
    }
    onSelFile(bFile);


    ui.comboBox_bin->setCurrentIndex(task.ftp_transferMode);

    ui.comboBox_passive->setCurrentIndex(task.ftp_connectMode);


    ui.le_RelvPath->setText(task.rltvPath);
    ui.lineEdit_3->setText(task.dispatch);
    ui.lineEdit_4->setText(task.fileTemplate);
    ui.lineEdit_7->setText(task.ip);
    ui.lineEdit_8->setText(QString::number(task.port));
    ui.lineEdit_5->setText(task.loginUser);
    ui.lineEdit_6->setText(task.loginPass);
    ui.checkBox_2->setChecked(task.subdirFlag);
//    ui.checkBox_6->setChecked(task.moveFlag);

    // 0:所有，1:30分钟，2：1小时，3：2小时，4：3小时，5：6小时，6：12小时，7：1天，8：2天，9：3天
    int index = 0;
    bool enterUserDefineItem = false;
    switch (task.col_timerange)
    {
    case -1:
        index = 0;
        break;
    case 30:
        index = 1;
        break;
    case 60:
        index = 2;
        break;
    case 60*2:
        index = 3;
        break;
    case 60*3:
        index = 4;
        break;
    case 60*6:
        index = 5;
        break;
    case 60*12:
        index = 6;
        break;
    case 60*24:
        index = 7;
        break;
    case 60*48:
        index = 8;
        break;
    case 60*72:
        index = 9;
        break;
    default:
        enterUserDefineItem = true;
        break;
    }

    if (enterUserDefineItem)
    {
        ui.col_time_range->setCurrentText(QString("%1").arg(task.col_timerange) + QStringLiteral("分钟"));
    }
    else
    {
        ui.col_time_range->setCurrentIndex(index);
    }


    ui.recordLatestTime->setChecked(task.recordLatestTime);

    m_task.dirID = task.dirID;
    // 临时屏蔽
//     m_selUser.dirID = task.dirID;
//     DataBase::getInstance()->QueryUserInfo(m_selUser);
//     QTableWidgetItem *pItem = new QTableWidgetItem(QStringLiteral("分发用户："));
//     ui.tableWidget->setItem(0, 0, pItem);
//     pItem = new QTableWidgetItem(m_selUser.user.userName);
//     ui.tableWidget->setItem(0, 1, pItem);
//     pItem = new QTableWidgetItem(m_selUser.rltvPath);
//     ui.tableWidget->setItem(0, 2, pItem);

    ui.comboBox_sendUser->setCurrentText(getSendUserInfoFromDirID(task.dirID).user.userName);


    //for (int i=0; i<tUser.lstUser.size(); ++i)
    //{
    //    const CollectUser &cUser = tUser.lstUser.at(i);
    //    QListWidgetItem *pItem = new QListWidgetItem();
    //    ui.listWidget->addItem(pItem);

    //    UserItem *pUser = new UserItem(ui.listWidget, cUser.user.userID, cUser.user.userName, cUser.rltvPath);
    //    connect(pUser, SIGNAL(onDelete(const QString &)), this, SLOT(onSendUserDel(const QString &)));
    //    ui.listWidget->setItemWidget(pItem, pUser);
    //    pItem->setSizeHint(QSize(pUser->rect().width(), pUser->rect().height()));
    //}
}

void CollectSetDlg::onSendUserDel(const QString &user)
{
    //if (!m_task.dirID.isEmpty())
    //{
    //    for (int i = 0; i < m_selUser.lstUser.size(); ++i)
    //    {
    //        const CollectUser &oUser = m_selUser.lstUser.at(i);
    //        if (oUser.user.userID == user)
    //        {
    //            m_selUser.lstUser.removeAt(i);
    //            break;
    //        }
    //    }
    //    DataBase::getInstance()->DeleteCollectSenderUser(m_task.dirID, user);
    //}

    //ui.listWidget->takeItem(0);
}

void CollectSetDlg::onRegExpTest()
{
    CRegExpDlg oRegDlg;
    oRegDlg.exec();
}

CollectUser CollectSetDlg::getSendUserInfoFromName(const QString &name)
{
    CollectUser retUser;
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser) || lstUser.size() == 0)
    {
        return retUser;
    }
    for (int i = 0; i < lstUser.size(); ++i)
    {
        if (name == lstUser[i].userName)
        {
            retUser.user = lstUser[i];
            break;
        }
    }

    return retUser;
}

CollectUser CollectSetDlg::getSendUserInfoFromDirID(const QString &CollectDirId)
{
    CollectUser retUser;

    TaskUser user;
    user.taskID = CollectDirId;
    if (!DataBase::getInstance()->QueryUserInfo(user))
    {
        return retUser;
    }

    if (user.lstUser.size() > 0)
    {
        retUser = user.lstUser[0];
    }

    return retUser;
}

void CollectSetDlg::onRemoteColTest()
{

    // 文件目录收集
    //qDebug() << ui.radFile->isChecked() << ui.radFtp->isChecked();

    if (ui.radFile->isChecked())
    {
        QUrl url = QUrl::fromLocalFile(CPathBuilder::getFinalPathFromUrl(ui.le_RelvPath->text()));
        if (url.isLocalFile())
        {
            QDir qdir(ui.le_RelvPath->text());
            if (qdir.exists())
            {
                emit testok();
                return;
            }
        }

        emit testfail();
    }
    else
    {

        CURL *curl = curl_easy_init();

        if (1 == ui.comboBox_bin->currentIndex())
        {
            // 使用ascii
            curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1L);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 0);
        }

        // 使用主动，默认为被动
        if (1 == ui.comboBox_passive->currentIndex())
        {
            curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
        }


        char url[100] = { 0 };
        char usrPwd[100] = { 0 };
        sprintf(url, "ftp://%s:%d", ui.lineEdit_7->text().toStdString().c_str(), ui.lineEdit_8->text().toInt());
        sprintf(usrPwd, "%s:%s", ui.lineEdit_5->text().toStdString().c_str(), ui.lineEdit_6->text().toStdString().c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, usrPwd);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        CURLcode res = curl_easy_perform(curl);
        if (CURLE_OK != res)
        {
            emit testfail();
            curl_easy_cleanup(curl);
            return;

        }
        curl_easy_cleanup(curl);
        emit testok();
    }

}

void CollectSetDlg::onTestOk()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::green);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("连接成功"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void CollectSetDlg::onTestFail()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::red);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("连接失败"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void CollectSetDlg::onTestResultTimeout()
{
    ui.label_result->setText("");
}

void CollectSetDlg::onWaitForTest()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("正在测试网络连接，请稍后..."));
    QTimer::singleShot(300, this, SLOT(onRemoteColTest()));
}

QString CollectSetDlg::getSendUserNameFromDirID(const QString &CollectDirId)
{
    CollectUser oUser = getSendUserInfoFromDirID(CollectDirId);
    return oUser.user.userName;
}

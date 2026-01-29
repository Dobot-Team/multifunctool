#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProtocolClass.h"
#include <stdio.h>
#include <iostream>
extern "C"
{
#include "UpdataBase.h"
}

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MultiFuncTool Ver 1.1.0");
    ProtocolTypeAddItem();
    qDebug() << ui->ProtocolType->currentText() << ui->ProtocolType->currentIndex();
    ui->ProtocolType->setCurrentText("DobotV3");
    qDebug() << ui->ProtocolType->currentText() << ui->ProtocolType->currentIndex();
    DataTransShowOneTab(ui->ProtocolType->currentText());
    qDebug() << ui->ProtocolType->currentText();
    ui->DataTrans->setEnabled(false);
    pBGPTimer = new QTimer(this);
    connect(pBGPTimer, SIGNAL(timeout()), this, SLOT(BackGroundProgram()));
    pBGPTimer->start(10);
    ui->ScrollDisplay->setChecked(true);
    ui->HexShow->setChecked(true);

    pTimeCNT = new QTimer(this);
    connect(pTimeCNT, SIGNAL(timeout()), this, SLOT(TimeCNT()));
    pTimeCNT->start(5);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pBGPTimer;
    delete pTimeCNT;
    if (pDataTransTab != NULL)
    {
        delete pDataTransTab;
        pDataTransTab = NULL;
    }
}

/*************************************************************
** Function name:       MainWindow::BackGroundProgram
** Descriptions:        后台运行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::BackGroundProgram(void)
{
    return;
}

/*************************************************************
** Function name:       MainWindow::TimeCNT
** Descriptions:        时钟计数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::TimeCNT(void)
{
    UpdataBase_TIMECNT(5);
}

/*************************************************************
** Function name:       MainWindow::DataTransShowOneTab
** Descriptions:        显示DataTrans的指定Tab
** Input parameters:    pTab：需要显示的Tab指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::DataTransShowOneTab(const QString &pTab)
{
    static uint8_t Mark = 0;
    static uint16_t dataTransCount;
    if (Mark == 0)
    {
        dataTransCount = ui->DataTrans->count();
        pDataTransTab = new QWidgetTabName[dataTransCount];
        for (uint16_t cir = 0; cir < dataTransCount; cir++)
        {
            pDataTransTab[cir].tab = ui->DataTrans->widget(cir);
            pDataTransTab[cir].name = ui->DataTrans->tabText(cir);
        }
        Mark = 1;
    }
    while (ui->DataTrans->count())
    {
        ui->DataTrans->removeTab(0);
    }
    for (uint16_t cir1 = 0; cir1 < dataTransCount; cir1++)
    {
        if (pDataTransTab[cir1].name == pTab)
        {
            ui->DataTrans->addTab(pDataTransTab[cir1].tab, pDataTransTab[cir1].name);
        }
    }
}

/*************************************************************
** Function name:       MainWindow::ProtocolTypeAddItem
** Descriptions:        添加 ProtocolType 的项目
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::ProtocolTypeAddItem(void)
{
    qDebug() << "ProtocolTypeAddItem";
    for (uint16_t cir = 0; cir < ui->DataTrans->count(); cir++)
    {
        ui->ProtocolType->addItem(ui->DataTrans->tabText(cir));
    }
    ProtocolTypeAddItemMark = true;
}

/*************************************************************
** Function name:       MainWindow::IsComOpen
** Descriptions:        检查串口是否开启
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
bool MainWindow::IsComOpen(void)
{
    if (ui->ComOpen->text() == "开启")
    {
        return false;
    }
    return true;
}

/*************************************************************
** Function name:       ComRefresh
** Descriptions:        串口刷新
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::ComRefresh(QStringList list)
{
    ui->Com->clear();
    ui->Com->addItems(list);
}

/*************************************************************
** Function name:       MainWindow::ComStatus
** Descriptions:        串口状态
** Input parameters:    status：true 打开，false 关闭
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::ComStatus(bool status)
{
    if (status == true)
    {
        ui->ComOpen->setText("关闭");
        ui->Com->setEnabled(false);
        ui->BaudRate->setEnabled(false);
        ui->DataBit->setEnabled(false);
        ui->VerifyBit->setEnabled(false);
        ui->StopBit->setEnabled(false);
        ui->DataTrans->setEnabled(true);
        ui->DataShow->setReadOnly(true);
        ui->DataShow->moveCursor(QTextCursor::End);
    }
    else
    {
        ui->ComOpen->setText("开启");
        ui->Com->setEnabled(true);
        ui->BaudRate->setEnabled(true);
        ui->DataBit->setEnabled(true);
        ui->VerifyBit->setEnabled(true);
        ui->StopBit->setEnabled(true);
        ui->DataTrans->setEnabled(false);
        ui->DataShow->setReadOnly(false);
    }
}

/*************************************************************
** Function name:       OpenFirmFile
** Descriptions:        打开固件文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::OpenFirmFile(void)
{
    // 在串口打开时发出信号
    ProtocolTypeChanged();
    QString fileAddr = QFileDialog::getOpenFileName(this, tr("open file"), " ", tr("ALL(*.bin *.Dfirm);;bin文件(*.bin);;Dfirm文件(*.Dfirm)"));
    if (fileAddr.contains(".bin", Qt::CaseSensitive) == true)
    {
        emit FirmwareShow(fileAddr);
    }
    else if (fileAddr.contains(".Dfirm", Qt::CaseSensitive) == true)
    {
        ShowFirmAddr(fileAddr);
    }
    else
    {
        qDebug() << "Please choose firmware file";
    }
}

void MainWindow::BurnV3Firmware(QString fileAddr, QString version)
{
    // 命令号打开
    ProtocolTypeChanged();
    if (fileAddr.contains(".bin", Qt::CaseSensitive) == true)
    {
        emit FirmwareShow(fileAddr, version);
        // todo:貌似有错
    }
    else if (fileAddr.contains(".Dfirm", Qt::CaseSensitive) == true)
    {
        ShowFirmAddr(fileAddr);
        startUpdate_slot();
    }
    else
    {
        qDebug() << "Please choose firmware file";
    }
}

/*************************************************************
** Function name:       MainWindow::OneAscallToHex
** Descriptions:        将一个ASCALL转成16进制数字
** Input parameters:    None
** Output parameters:   None
** Returned value:      0:成功
**                      1:空字符
**                      2:错误字符
** Remarks:             None
*************************************************************/
uint8_t MainWindow::OneAscallToHex(uint8_t ascall, uint8_t *pHex)
{
    if (ascall == ' ')
    {
        return 1;
    }
    else if (ascall >= '0' && ascall <= '9')
    {
        *pHex = ascall - '0';
        return 0;
    }
    else if (ascall >= 'A' && ascall <= 'F')
    {
        *pHex = ascall - 'A' + 10;
        return 0;
    }
    else if (ascall >= 'a' && ascall <= 'f')
    {
        *pHex = ascall - 'a' + 10;
        return 0;
    }
    return 2;
}

/*************************************************************
** Function name:       MainWindow::StringToHex
** Descriptions:        将字符串转成16进制
** Input parameters:    string：需要转换的字符串
**                      pHex:16进制数据指针
**                      Len:数据指针长度
**                      pCHexNum:成功转换的个数
** Output parameters:   None
** Returned value:      0:成功
**                      1:出现错误的字符格式
** Remarks:             None
*************************************************************/
uint8_t MainWindow::StringToHex(QString string, uint8_t *pHex, uint16_t hexLen, uint16_t *pCHexNum)
{
    uint16_t stringNum = string.toLatin1().length();
    uint8_t step = 0;
    uint8_t hex = 0;
    uint8_t result = 0;
    uint8_t returnValue = 0;
    uint8_t index = 0;
    *pCHexNum = 0;
    while ((stringNum--) && (*pCHexNum < hexLen))
    {
        result = OneAscallToHex(string.constData()[index++].toLatin1(), &hex);
        if (result == 2)
        {
            returnValue = 1;
            break;
        }
        else if (result == 1)
        {
            if (step == 1)
            {
                pHex++;
                (*pCHexNum)++;
                step = 0;
            }
            continue;
        }
        if (step == 0)
        {
            *pHex = hex;
            step++;
        }
        else
        {
            *pHex = (*pHex << 4) | hex;
            pHex++;
            (*pCHexNum)++;
            step = 0;
        }
    }
    if (step == 1)
    {
        pHex++;
        (*pCHexNum)++;
        step = 0;
    }
    return returnValue;
}

/*************************************************************
** Function name:       MainWindow::StringToUint8
** Descriptions:        将字符转成Uint8的数据
** Input parameters:    string：需要转换的字符
**                      pData：数据指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t MainWindow::StringToUint8(QString string, uint8_t *pData)
{
    memcpy(pData, string.toStdString().data(), string.toStdString().length());
    return string.toStdString().length();
}

/*************************************************************
** Function name:       MainWindow::StrToHexStr
** Descriptions:        把Str类型转成hexStr显示
** Input parameters:    string：需要转换的字符
**                      stringHex：转换后的字符
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::StrToHexStr(QString string, QString &stringHex)
{
    stringHex = string.toLatin1().toHex();
    uint16_t index = 1;
    while (string.length() != index)
    {
        stringHex.insert(index * 3 - 1, " ");
        index++;
    }
}

/*************************************************************
** Function name:       MainWindow::Uint8ToString
** Descriptions:        将Uint8数据转成字符
** Input parameters:    string：需要转换的字符
**                      pData：数据指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::Uint8ToHex(uint8_t *pData, uint16_t dataLen, QString &string)
{
    string.clear();
    uint8_t index = 0;
    while (index != dataLen)
    {
        QByteArray qByte((char *)(pData + index), 1);
        string.append(qByte.toHex().toUpper());
        string += " ";
        index++;
    }
}

/*************************************************************
** Function name:       MainWindow::Uint8ToString
** Descriptions:        将Uint8数据转成字符
** Input parameters:    string：需要转换的字符
**                      pData：数据指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::Uint8ToString(uint8_t *pData, uint16_t dataLen, QString &string)
{
    string.clear();
    QByteArray qByte((char *)pData, dataLen);
    string.append(qByte);
}

/*************************************************************
** Function name:       MainWindow::DataShow
** Descriptions:        信号处理
** Input parameters:    pData：数据指针
**                      dataLen：数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::DataShow(uint8_t *pData, uint16_t dataLen)
{
    QString string;
    string.clear();
    if (ui->HexShow->isChecked())
    {
        Uint8ToHex(pData, dataLen, string);
    }
    else
    {
        Uint8ToString(pData, dataLen, string);
    }
    if (ui->ProtocolType->currentText() == "None")
    {
        ui->DataShow->insertPlainText(string);
        return;
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString showInfo = current_date_time.toString("[hh:mm:ss.zzz] ");
    showInfo += string;
    ui->DataShow->append(showInfo);
}

/*************************************************************
** Function name:       MainWindow::InfoShow
** Descriptions:        信息显示
** Input parameters:    info：信息数据指针
**                      len：信息长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::InfoShow(QString info)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString showInfo = current_date_time.toString("[hh:mm:ss.zzz] ");
    showInfo += info;
    ui->DataShow->append(showInfo);
}

/*************************************************************
** Function name:       MainWindow::FirmUpdataSchedule
** Descriptions:        固件进度
** Input parameters:    pc：百分比
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::FirmUpdataSchedule(float pc)
{
    if (ui->ProtocolType->currentText() == "Dobot")
    {
        ui->DobotProgress->setValue(pc);
    }
    else if (ui->ProtocolType->currentText() == "DobotV3")
    {
        ui->DobotV3Progress->setValue(pc);
        QTextStream cout(stdout); //绑定cout到标准输出
        cout.setCodec("UTF-8");

        cout << pc << endl;
    }
}

/*************************************************************
** Function name:       MainWindow::FirmUpdataSchedule
** Descriptions:        固件进度
** Input parameters:    pc：百分比
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::FirmUdataFinish(void)
{
    if (ui->ProtocolType->currentText() == "Dobot")
    {
        ui->Dobot_FirmUpdata->setText("固件升级");
    }
    else if (ui->ProtocolType->currentText() == "DobotV3")
    {
        ui->DobotV3_FirmUpdata->setText("固件升级");
    }
}

/*************************************************************
** Function name:       MainWindow::ShowFirmAddr
** Descriptions:        显示固件地址
** Input parameters:    fileAddr:文件路径
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::ShowFirmAddr(QString fileAddr)
{
    if (ui->ProtocolType->currentText() == "Dobot")
    {
        ui->Dobot_FirmName->setText(fileAddr);
    }
    else if (ui->ProtocolType->currentText() == "DobotV3")
    {
        ui->DobotV3_FirmName->setText(fileAddr);
    }
    ui->DobotV3_FirmName->setText(fileAddr);
    qDebug() << __FUNCTION__ << fileAddr;
}

/*************************************************************
** Function name:       MainWindow::Warming
** Descriptions:        报警提示
** Input parameters:    message：报警提示的信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::Warming(QString message)
{
    //    cout<<"Warming"<<message<<endl;
    QTextStream cout(stdout); //绑定cout到标准输出
    cout.setCodec("UTF-8");

    cout << message << endl;
    QMessageBox msgBox(QMessageBox::Warning, tr("Warming"),
                       message, 0, this);
    msgBox.addButton(tr("关闭"), QMessageBox::RejectRole);
    msgBox.exec();
}

/*************************************************************
** Function name:       MainWindow::on_ProtocolType_currentIndexChanged
** Descriptions:        ProtocolType 下标变化，修改
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_ProtocolType_currentIndexChanged(const QString &arg1)
{
    qDebug() << __FUNCTION__ << ProtocolTypeAddItemMark;
    if (ProtocolTypeAddItemMark == true)
    {
        //    if(ui->ProtocolType->currentText() == "DobotV3"){
        DataTransShowOneTab(arg1);
        qDebug() << "!!!" << __FUNCTION__;
        emit ProtocolOpenExec(ui->ProtocolType->currentText());
    }
}

void MainWindow::ProtocolTypeChanged()
{
    qDebug() << __FUNCTION__ << ProtocolTypeAddItemMark;
    if (ProtocolTypeAddItemMark == true)
    {
        //    if(ui->ProtocolType->currentText() == "DobotV3"){
        //        DataTransShowOneTab(arg1);
        qDebug() << "!!!" << __FUNCTION__;
        emit ProtocolOpenExec(ui->ProtocolType->currentText());
    }
}

/*************************************************************
** Function name:       MainWindow::on_ComOpen_clicked
** Descriptions:        串口开启或者关闭
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_ComOpen_clicked()
{
    if (IsComOpen() == false)
    {
        emit ComOpen(ui->Com->currentText(),
                     ui->BaudRate->currentText().toInt(),
                     (QSerialPort::DataBits)ui->DataBit->currentText().toInt(),
                     (QSerialPort::Parity)ui->VerifyBit->currentText().toInt(),
                     (QSerialPort::StopBits)ui->StopBit->currentText().toInt());
    }
    else
    {
        emit ComClose();
    }
}

/*************************************************************
** Function name:       MainWindow::on_Dobot_FirmFile_clicked
** Descriptions:        打开固件升级文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_Dobot_FirmFile_clicked()
{
    OpenFirmFile();
}

/*************************************************************
** Function name:       MainWindow_STRINGTOHEX
** Descriptions:        将字符转成HEX
** Input parameters:    string:(QString)需要转换的字符
**                      pHex：接收字符的指针
**                      hexLen：字符指针长度
**                      pCHexNum：转换成Hex的个数指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define MainWindow_STRINGTOHEX(string, pHex, hexLen, pCHexNum) \
    if (StringToHex(string, pHex, hexLen, pCHexNum) == 1)      \
    {                                                          \
        Warming("字符格式错误");                               \
        return;                                                \
    }

/*************************************************************
** Function name:       MainWindow::on_pushButton_5_clicked
** Descriptions:        Dobot发送按钮
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_pushButton_5_clicked()
{
    uint16_t cHexNum = 0;
    MainWindow_STRINGTOHEX(ui->DobotCmdID->text(), &dobotSendData.ID, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotCmdRW->text(), &dobotSendData.rw, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotCmdIsQueue->text(), &dobotSendData.isQueue, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotCmdDevice->text(), &dobotSendData.device, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotCmdParam->text(), dobotSendData.param, mainwindow_DOBOTSDATA_PARAMLEN, &cHexNum);
    emit ProtocolSendData((uint8_t *)&dobotSendData, cHexNum + 3);
}

/*************************************************************
** Function name:       MainWindow::on_NoneClear_clicked
** Descriptions:        None类型协议数据清空
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_NoneClear_clicked()
{
    ui->NoneData->setText(NULL);
}

/*************************************************************
** Function name:       MainWindow::on_NoneSend_clicked
** Descriptions:        None协议类型数据发送
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_NoneSend_clicked()
{
    uint8_t dataLen = ui->NoneData->toPlainText().toLatin1().length();
    uint8_t *pData = new uint8_t[dataLen];
    StringToUint8(ui->NoneData->toPlainText(), pData);
    emit ProtocolSendData(pData, dataLen);
    delete pData;
}

/*************************************************************
** Function name:       MainWindow::on_DataShow_cursorPositionChanged
** Descriptions:        设置滚动显示
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_DataShow_cursorPositionChanged()
{
    if (ui->ScrollDisplay->isChecked())
    {
        ui->DataShow->moveCursor(QTextCursor::End);
    }
}

/*************************************************************
** Function name:       MainWindow::on_ClearDataShow_clicked
** Descriptions:        清空显示窗口
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_ClearDataShow_clicked()
{
    ui->DataShow->clear();
}

/*************************************************************
** Function name:       MainWindow::on_Dobot_FirmUpdata_clicked
** Descriptions:        Dobot 固件升级
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_Dobot_FirmUpdata_clicked()
{
    if (ui->Dobot_FirmUpdata->text() == "固件升级")
    {
        ui->Dobot_FirmUpdata->setText("取消升级");
        uint16_t cHexNum = 0;
        MainWindow_STRINGTOHEX(ui->DobotFirmID->text(), &dobotSendData.ID, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotFirmRW->text(), &dobotSendData.rw, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotFirmIsQueue->text(), &dobotSendData.isQueue, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotFirmDevice->text(), &dobotSendData.device, 1, &cHexNum);
        memset(fileAddr, 0, 300);
        StringToUint8(ui->Dobot_FirmName->text(), (uint8_t *)fileAddr);
        emit StartFirmUpdata(true, fileAddr, (uint8_t *)&dobotSendData);
    }
    else
    {
        ui->Dobot_FirmUpdata->setText("固件升级");
        emit StartFirmUpdata(false, NULL, NULL);
    }
}

/*************************************************************
** Function name:       on_DobotV3_Send_clicked
** Descriptions:        DobotV3 发送数据
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_DobotV3_Send_clicked()
{
    uint16_t cHexNum = 0;
    memset(&dobotV3SendData, 0, sizeof(mainwindow_DobotV3SData));
    MainWindow_STRINGTOHEX(ui->DobotV3_Ver->text(), &dobotV3SendData.Ver, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_NAck->text(), &dobotV3SendData.Nack, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_IsAck->text(), &dobotV3SendData.IsAck, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_RW->text(), &dobotV3SendData.RW, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CType->text(), &dobotV3SendData.CType, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_ENC->text(), &dobotV3SendData.ENC, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SEQ->text(), &dobotV3SendData.SEQ, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SEQNum->text(), (uint8_t *)&dobotV3SendData.SEQNum, 4, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SRC->text(), &dobotV3SendData.SRC, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_DES->text(), &dobotV3SendData.DES, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CmdSet->text(), &dobotV3SendData.CmdSet, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CmdId->text(), &dobotV3SendData.CmdId, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_PayLoad->text(), dobotV3SendData.PayLoad, mainwindow_DOBOTV3SDATA_PARAMLEN, &cHexNum);
    emit ProtocolSendData((uint8_t *)&dobotV3SendData, cHexNum + (dobotV3SendData.PayLoad - &dobotV3SendData.Ver));
}

/*************************************************************
** Function name:       on_DobotV3_FirmUpdata_clicked
** Descriptions:        DobotV3 固件升级
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_DobotV3_FirmUpdata_clicked()
{
    if (ui->DobotV3_FirmUpdata->text() == "固件升级")
    {
        ui->DobotV3_FirmUpdata->setText("取消升级");
        uint16_t cHexNum = 0;
        memset(&dobotV3SendData, 0, sizeof(mainwindow_DobotV3SData));
        MainWindow_STRINGTOHEX(ui->DobotV3_Ver_U->text(), &dobotV3SendData.Ver, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_NAck_U->text(), &dobotV3SendData.Nack, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_IsAck_U->text(), &dobotV3SendData.IsAck, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_RW_U->text(), &dobotV3SendData.RW, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_CType_U->text(), &dobotV3SendData.CType, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_ENC_U->text(), &dobotV3SendData.ENC, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_SEQ_U->text(), &dobotV3SendData.SEQ, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_SEQNum_U->text(), (uint8_t *)&dobotV3SendData.SEQNum, 4, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_SRC_U->text(), &dobotV3SendData.SRC, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_DES_U->text(), &dobotV3SendData.DES, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_CmdSet_U->text(), &dobotV3SendData.CmdSet, 1, &cHexNum);
        MainWindow_STRINGTOHEX(ui->DobotV3_CmdId_U->text(), &dobotV3SendData.CmdId, 1, &cHexNum);
        memset(fileAddr, 0, 300);
        StringToUint8(ui->DobotV3_FirmName->text(), (uint8_t *)fileAddr);
        qDebug() << __FUNCTION__ << ui->DobotV3_FirmName->text() << fileAddr;
        emit StartFirmUpdata(true, fileAddr, (uint8_t *)&dobotV3SendData);
    }
    else
    {
        ui->DobotV3_FirmUpdata->setText("固件升级");
        emit StartFirmUpdata(false, NULL, NULL);
    }
}

void MainWindow::startUpdate_slot()
{
    uint16_t cHexNum = 0;
    memset(&dobotV3SendData, 0, sizeof(mainwindow_DobotV3SData));
    ui->DobotV3_Ver_U->setText("11");
    ui->DobotV3_NAck_U->setText("0");
    ui->DobotV3_IsAck_U->setText("0");
    ui->DobotV3_RW_U->setText("0");
    ui->DobotV3_CType_U->setText("0");
    ui->DobotV3_ENC_U->setText("0");
    ui->DobotV3_SEQ_U->setText("0");
    ui->DobotV3_SEQNum_U->setText("0");
    ui->DobotV3_SRC_U->setText("0");
    ui->DobotV3_DES_U->setText("4");
    ui->DobotV3_CmdSet_U->setText("0");
    ui->DobotV3_CmdId_U->setText("15");
    MainWindow_STRINGTOHEX(ui->DobotV3_Ver_U->text(), &dobotV3SendData.Ver, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_NAck_U->text(), &dobotV3SendData.Nack, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_IsAck_U->text(), &dobotV3SendData.IsAck, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_RW_U->text(), &dobotV3SendData.RW, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CType_U->text(), &dobotV3SendData.CType, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_ENC_U->text(), &dobotV3SendData.ENC, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SEQ_U->text(), &dobotV3SendData.SEQ, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SEQNum_U->text(), (uint8_t *)&dobotV3SendData.SEQNum, 4, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_SRC_U->text(), &dobotV3SendData.SRC, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_DES_U->text(), &dobotV3SendData.DES, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CmdSet_U->text(), &dobotV3SendData.CmdSet, 1, &cHexNum);
    MainWindow_STRINGTOHEX(ui->DobotV3_CmdId_U->text(), &dobotV3SendData.CmdId, 1, &cHexNum);
    memset(fileAddr, 0, 300);
    StringToUint8(ui->DobotV3_FirmName->text(), (uint8_t *)fileAddr);
    qDebug() << __FUNCTION__ << ui->DobotV3_FirmName->text() << fileAddr;
    emit StartFirmUpdata(true, fileAddr, (uint8_t *)&dobotV3SendData);
}

/*************************************************************
** Function name:       on_DobotV3_FirmFile_clicked
** Descriptions:        打开DobotV3固件文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MainWindow::on_DobotV3_FirmFile_clicked()
{
    OpenFirmFile();
}

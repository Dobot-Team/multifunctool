#include "firmwareui.h"
#include "ui_firmwareui.h"
#include <QTime>
FirmwareUI::FirmwareUI(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::FirmwareUI)
{
    ui->setupUi(this);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

FirmwareUI::~FirmwareUI()
{
    delete ui;
}

/*************************************************************
** Function name:       MainWindow::Warming
** Descriptions:        报警提示
** Input parameters:    message：报警提示的信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void FirmwareUI::Warming(QString message)
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Warming"),
                       message, 0, this);
    msgBox.addButton(tr("关闭"), QMessageBox::RejectRole);
    msgBox.exec();
}

/*************************************************************
** Function name:       FirmwareUI::Show
** Descriptions:        窗口显示
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void FirmwareUI::Show(QString file)
{
    qDebug() << "FirmwareUI" << __FUNCTION__;
    this->setWindowTitle("固件信息");
    this->show();
    ui->TextFileAddr->setText(file);
}

void FirmwareUI::Show(QString file, QString version)
{
    qDebug() << "FirmwareUI" << __FUNCTION__;
    this->setWindowTitle("固件信息");
    //    this->show();
    // 相当于填完固件信息
    ui->TextFileAddr->setText(file);
    ui->DeviceName->setText("MagicianGO");
    ui->DeviceAddr->setText("4");
    ui->Version1->setText(version.at(0));
    ui->Version2->setText(version.at(1));
    ui->Version3->setText(version.at(2));
    ui->Version4->setText(version.at(3));
    ui->Version5->setText(version.at(4));
    ui->Version6->setText(version.at(5));
    ui->Version7->setText(version.at(6));
    ui->Version8->setText(version.at(7));
    ui->InfoAddr->setText("800B000");
    ui->ProAddr->setText("800C000");
    // 相当于点击确认
    if (CheckFirmInfo() == false)
    {
        qDebug() << "firmware information error!";
        return;
    }
    // 相当于设置dfirm文件的路径
    emit ReturnFirmAddr(dfirmFileAddr);
    // 相当于点击固件升级,延时几秒
    QObject *obj = new QObject(this);
    QTimer::singleShot(3000, obj, [=]()
                       {
                           obj->deleteLater();
                           emit startUpdate_signal();
                       });

    this->reject();
}

/*************************************************************
** Function name:       FirmwareUI::StringToUint8
** Descriptions:        将字符转成Uint8的数据
** Input parameters:    string：需要转换的字符
**                      pData：数据指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t FirmwareUI::StringToUint8(QString string, uint8_t *pData)
{
    uint16_t cir = string.toLatin1().length();
    uint16_t index = 0;
    while (cir != index)
    {
        pData[index] = string.constData()[index++].toLatin1();
    }
    return cir;
}

/*************************************************************
** Function name:       FirmwareUI::OneAscallToHex
** Descriptions:        将一个ASCALL转成16进制数字
** Input parameters:    None
** Output parameters:   None
** Returned value:      0:成功
**                      1:空字符
**                      2:错误字符
** Remarks:             None
*************************************************************/
uint8_t FirmwareUI::OneAscallToHex(uint8_t ascall, uint8_t *pHex)
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
** Function name:       FirmwareUI::StringToHex
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
uint8_t FirmwareUI::StringToHex(QString string, uint8_t *pHex, uint16_t hexLen, uint16_t *pCHexNum)
{
    uint16_t stringNum = string.toLatin1().length();
    uint8_t hex = 0;
    uint8_t result = 0;
    uint32_t changeAscall = 0;
    uint8_t returnValue = 0;
    while ((stringNum) && (changeAscall < (hexLen * 2)))
    {
        stringNum--;
        result = OneAscallToHex(string.constData()[stringNum].toLatin1(), &hex);
        if (result == 2)
        {
            returnValue = 1;
            break;
        }
        else if (result == 1)
        {
            continue;
        }
        if ((changeAscall % 2) == 1)
        {
            pHex[changeAscall / 2] = pHex[changeAscall / 2] + (hex << 4);
        }
        else
        {
            pHex[changeAscall / 2] = hex;
        }
        changeAscall++;
    }
    *pCHexNum = changeAscall / 2 + 1;
    return returnValue;
}

/*************************************************************
** Function name:       FirmwareUI_STHWITHWARM
** Descriptions:        string to hex 带预警信息显示
** Input parameters:    qstring：需要转换的字符串
**                      pdata:接收转换的数据指针
**                      len：数据指针长度
**                      warm:警告信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define FirmwareUI_STHWITHWARM(qstring, pdata, len, returnLen, warm)  \
    if (StringToHex(qstring, (uint8_t *)pdata, len, &returnLen) == 1) \
    {                                                                 \
        Warming(warm);                                                \
        return false;                                                 \
    }

/*************************************************************
** Function name:       FirmwareUI_ISEMPTYWARM
** Descriptions:        检查字符串是否为空
** Input parameters:    qstring：需要检查的字符串
**                      warm:警告信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define FirmwareUI_ISEMPTYWARM(qstring, wram) \
    if (qstring.isEmpty())                    \
    {                                         \
        Warming(wram);                        \
        return false;                         \
    }

/*************************************************************
** Function name:       FirmwareUI::CheckFirmInfo
** Descriptions:        检查固件信息
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
bool FirmwareUI::CheckFirmInfo(void)
{
    FirmwareUI_ISEMPTYWARM(ui->DeviceName->text(), "DeviceName is NULL");
    FirmwareUI_ISEMPTYWARM(ui->DeviceAddr->text(), "DeviceAddr is NULL");
    if (ui->Version1->text().isEmpty() && ui->Version2->text().isEmpty() && ui->Version3->text().isEmpty() && ui->Version4->text().isEmpty() && ui->Version5->text().isEmpty() && ui->Version6->text().isEmpty() && ui->Version7->text().isEmpty() && ui->Version8->text().isEmpty())
    {
        Warming("Version is NULL");
        return false;
    }
    FirmwareUI_ISEMPTYWARM(ui->InfoAddr->text(), "InfoAddr is NULL");
    FirmwareUI_ISEMPTYWARM(ui->ProAddr->text(), "ProAddr is NULL");

    uint8_t *pDevName = new uint8_t[ui->DeviceName->text().length()];
    uint32_t deviceAddr = 0;
    uint8_t version[8];
    uint32_t infoAddr = 0;
    uint32_t proAddr = 0;
    uint16_t changeNum = 0;

    StringToUint8(ui->DeviceName->text(), pDevName);
    FirmwareUI_STHWITHWARM(ui->DeviceAddr->text(), &deviceAddr, 4, changeNum, "deviceAddr Format Err");
    FirmwareUI_STHWITHWARM(ui->Version1->text(), &version[0], 1, changeNum, "Version1 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version2->text(), &version[1], 1, changeNum, "Version2 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version3->text(), &version[2], 1, changeNum, "Version3 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version4->text(), &version[3], 1, changeNum, "Version4 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version5->text(), &version[4], 1, changeNum, "Version5 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version6->text(), &version[5], 1, changeNum, "Version6 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version7->text(), &version[6], 1, changeNum, "Version7 Format Err");
    FirmwareUI_STHWITHWARM(ui->Version8->text(), &version[7], 1, changeNum, "Version8 Format Err");
    FirmwareUI_STHWITHWARM(ui->InfoAddr->text(), &infoAddr, 4, changeNum, "infoAddr Format Err");
    FirmwareUI_STHWITHWARM(ui->ProAddr->text(), &proAddr, 4, changeNum, "proAddr Format Err");

    QString binAddr = ui->TextFileAddr->text();
    QString dfirmAddr = binAddr;
    dfirmAddr.replace(".bin", ".Dfirm", Qt::CaseSensitive);
    qDebug() << "firmMake";
    if (updataClass.FirmMake(binAddr, dfirmAddr, (char *)pDevName, deviceAddr, version, infoAddr, proAddr) == false)
    {
        qDebug() << "固件制作失败";
        Warming("固件制作失败");
        return false;
    }
    dfirmFileAddr = dfirmAddr;
    delete pDevName;
    return true;
}

void FirmwareUI::on_Sure_clicked()
{
    if (CheckFirmInfo() == false)
    {
        return;
    }
    // 相当于设置dfirm文件的路径
    emit ReturnFirmAddr(dfirmFileAddr);
    this->reject();
}

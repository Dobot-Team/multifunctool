/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataClass.cpp
** Latest modified date:    2020-04-29
** Latest version:          V1.0.0
** Description:             固件更新类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-29
** Version:                 V1.0.0
** Descriptions:            固件更新类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "UpdataClass.h"

UpdataClass::UpdataClass(void)
{
}

UpdataClass::~UpdataClass()
{
}

/*************************************************************
** Function name:       Init
** Descriptions:        用户初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::UserInit(void)
{
    return;
}

/*************************************************************
** Function name:       Init
** Descriptions:        初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::Init(void)
{
    UserInit();
}

/*************************************************************
** Function name:       UserExec
** Descriptions:        用户执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::UserExec(void)
{
    return;
}

/*************************************************************
** Function name:       Exec
** Descriptions:        后台执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::Exec(void)
{
    if (OpenExecMark == false)
    {
        return;
    }
    UserExec();
    return;
}

/*************************************************************
** Function name:       UpdataClass::FileOpen
** Descriptions:        打开文件
** Input parameters:    fileAddr：打开文件的地址
** Output parameters:   None
** Returned value:      true，打开成功，false，打开失败
** Remarks:             None
*************************************************************/
bool UpdataClass::FileOpen(char *pFileAddr)
{
    firmFile.close();
    firmFile.setFileName(pFileAddr);
    firmFile.open(QIODevice::ReadOnly);
    if (firmFile.isOpen() == true)
    {
        return true;
    }
    return false;
}

/*************************************************************
** Function name:       UpdataClass::FileClose
** Descriptions:        关闭文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::FileClose(void)
{
    firmFile.close();
}

/*************************************************************
** Function name:       UpdataClass::ReadFile
** Descriptions:        读取文件
** Input parameters:    pData:读取的数据指针
**                      dataLen：读取的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t UpdataClass::ReadFile(uint32_t Address, uint8_t *pData, uint16_t dataLen)
{
    if (firmFile.isOpen() == false)
    {
        EmitUpdataErr("Firmfile Open Fail,Please Check the firmfile address is correct");
        return 0;
    }
    firmFile.seek(Address);
    return firmFile.read((char *)pData, dataLen);
}

/*************************************************************
** Function name:       GetSize
** Descriptions:        获取文件大小
** Input parameters:    None
** Output parameters:   None
** Returned value:      文件大小
** Remarks:             None
*************************************************************/
uint32_t UpdataClass::GetFileSize(void)
{
    if (firmFile.isOpen() == false)
    {
        return 0;
    }
    return firmFile.size();
}

/*************************************************************
** Function name:       UpdataClass::UpdataStart
** Descriptions:        开始升级固件
** Input parameters:    firmFileAddr：固件名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::UserUpdataStart(char *fileAddr, uint8_t *pHead)
{
    return;
}

/*************************************************************
** Function name:       UpdataClass::UserUpdataFinish
** Descriptions:        结束固件升级
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::UserUpdataFinish(void)
{
    return;
}

/*************************************************************
** Function name:       UpdataClass::UpdataStartSolt
** Descriptions:        开始升级固件信号槽
** Input parameters:    isStart：true开始升级，false，结束升级
**                      pFirmFileAddr：固件名称指针
**                      pHead：协议头部信息指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::UpdataStartSolt(bool isStart, char *pFirmFileAddr, uint8_t *pHead)
{
    qDebug() << __FUNCTION__ << OpenExecMark << className;
    if (OpenExecMark == false)
    {
        return;
    }
    if (isStart == true)
    {
        qDebug() << __FUNCTION__;
        UserUpdataStart(pFirmFileAddr, pHead);
    }
    else
    {
        UserUpdataFinish();
    }
}

/*************************************************************
** Function name:       UpdataClass::FirmMake
** Descriptions:        制作固件
** Input parameters:    info：信息区域指针
**                      binAddr：固件bin文件地址
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
bool UpdataClass::FirmMake(QString binAddr, QString dfirmAddr,
                           char *pName,
                           uint32_t deviceAddr,
                           uint8_t *pVerison,
                           uint32_t infoAddr,
                           uint32_t proAddr)
{
    uint32_t infoSize = proAddr - infoAddr;
    qDebug() << __FUNCTION__ << infoSize << "start!";
    /* 打开binFile文件 */
    QFile binFile(binAddr);
    if (binFile.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    /* 打开firmFile文件 */
    QFile firmFile(dfirmAddr);
    firmFile.remove();
    if (firmFile.open(QIODevice::ReadWrite) == false)
    {
        return false;
    }
    firmFile.seek(infoSize); /* 预留info区域位置 */

    MD5_CTX md5; /* MD5校验 */
    MD5Init(&md5);
    char data[SIZE_OF_MD5_BLOCK] = {0};
    uint8_t verify[UpdataBase_PROVERIFYLEN];
    uint16_t len = 0;
    while (!binFile.atEnd())
    {
        len = binFile.read(data, SIZE_OF_MD5_BLOCK);
        firmFile.write(data, len);
        MD5Update(&md5, (uint8_t *)data, len);
    }
    MD5Final(&md5, verify);

    UpdataBase_FirmInfo info; /* 信息填充 */
    memset(&info, 0, sizeof(UpdataBase_FirmInfo));
    info.verifyLen = UpdataBase_PROVERIFYLEN;
    memcpy(info.verify, verify, UpdataBase_PROVERIFYLEN);
    info.nameLen = strlen(pName);
    if (info.nameLen >= 50)
    {
        info.nameLen = 50;
    }
    memcpy(info.name, pName, info.nameLen);
    info.deviceAddress = deviceAddr;
    memcpy(info.version, pVerison, 8);
    info.infoAddr = infoAddr;
    info.infoSize = infoSize;
    info.proAddr = proAddr;
    info.proSize = binFile.size();
    UpdataBase_InfoVerify(&info);
    char *pData = new char[infoSize];
    memset(pData, 0, infoSize);
    memcpy(pData, &info, sizeof(UpdataBase_FirmInfo));

    firmFile.seek(0);
    firmFile.write(pData, infoSize);
    firmFile.close();
    qDebug() << __FUNCTION__ << "finish!";
    delete pData;

    return true;
}

/*************************************************************
** Function name:       UpdataClass::EmitUpdataErr
** Descriptions:        发送固件升级错误信息信号
** Input parameters:    errInfo：错误信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::EmitUpdataErr(QString errInfo)
{
    QString err = "Updata Err: ";
    err.append(errInfo);
    qDebug() << __FUNCTION__ << errInfo;
    EmitUpdataFinish();
    emit UpdataInfoShow(err);
}

/*************************************************************
** Function name:       UpdataClass::EmitSchedule
** Descriptions:        发送固件升级错误信息信号
** Input parameters:    errInfo：错误信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::EmitSchedule(float pc, char *info)
{
    QString showInfo = "Updata Schedule: ";
    showInfo.append(QString::number(pc, 'f', 2));
    showInfo.append("  Info: ");
    showInfo.append(QString::fromUtf8(info, strlen(info)));
    emit UpdataInfoShow(showInfo);
    emit UpdataSchedule(pc);
}

/*************************************************************
** Function name:       UpdataClass::EmitUpdataFinish
** Descriptions:        发送固件升级结束信息信号
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::EmitUpdataFinish(void)
{
    emit UpdataFinishSignal();
}

/*************************************************************
** Function name:       ProtocolClass::OpenExec
** Descriptions:        开启执行
** Input parameters:    isOpen：true 开启，fasle 不开启
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataClass::OpenExec(QString Name)
{
    qDebug() << "UpdataClass::OpenExec" << Name;
    if (Name == className)
    {
        OpenExecMark = true;
    }
    else
    {
        OpenExecMark = false;
    }
}

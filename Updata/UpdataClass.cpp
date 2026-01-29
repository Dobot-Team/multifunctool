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

/*************************************************************
    全局变量
*************************************************************/

UpdataMaster_CB_EXPORT(gUpdataCB, NULL, NULL, NULL, NULL, NULL, NULL);

UpdataMaster_EXPORT(gUpdata, )

    /*************************************************************
    END 全局变量
*************************************************************/

    UpdataClass::UpdataClass(void)
{
}

UpdataClass::~UpdataClass()
{
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

    delete pData;
    return true;
}

/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataBase.c
** Latest modified date:    2020-04-25
** Latest version:          V1.0.0
** Description:             固件升级基础参数
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-25
** Version:                 V1.0.0
** Descriptions:            固件升级基础参数
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "UpdataBase.h"

uint32_t gUpdataBase_TimeCNT; /* 时间计数 */

/*************************************************************
    Verify MD5
*************************************************************/
#include "md5.h"
MD5_CTX gUpdataBase_MD5;
/*************************************************************
** Function name:       UpdataBase_VerifyInit
** Descriptions:        初始化
** Input parameters:    None
** Output parameters:   None
** Returned             None
*************************************************************/
void UpdataBase_ProVerifyInit(void)
{
    MD5Init(&gUpdataBase_MD5);
}

/*************************************************************
** Function name:       UpdataBase_DataVerify
** Descriptions:        数据校验，将需要校验的数据按照顺序写入
** Input parameters:    pData：需要校验的数据
**                      len：需要校验的数据长度
** Output parameters:   None
** Returned             None
*************************************************************/
void UpdataBase_ProDataVerify(uint8_t *pData, uint16_t len)
{
    MD5Update(&gUpdataBase_MD5, pData, len);
}

/*************************************************************
** Function name:       UpdataBase_VerifyFinal
** Descriptions:        校验结束
** Input parameters:    pVerify：存放校验结果指针
** Output parameters:   None
** Returned             None
*************************************************************/
void UpdataBase_ProVerifyFinal(uint8_t *pVerify)
{
    MD5Final(&gUpdataBase_MD5, pVerify);
}

/*************************************************************
    END Verify MD5
*************************************************************/

/*************************************************************
** Function name:       UpdataBase_PacketErrEncode
** Descriptions:        数据包错误代码编码
** Input parameters:    PSend：发送数据结构体指针
**                      Type：发送的数据类型
**                      Err：错误代码
** Output parameters:   None
** Returned value:      需要发送的数据包长度
** Remarks:             None
*************************************************************/
uint16_t UpdataBase_PacketErrEncode(PUpdataBase_Packet pPacket, uint8_t type, uint8_t err)
{
    pPacket->type = type;
    pPacket->err = err;
    pPacket->parmLen = 0;
    return UpdataBase_PACKETHEADSIZE;
}

/*************************************************************
** Function name:       UpdataBase_PacketEncode
** Descriptions:        数据包编码
** Input parameters:    pPacket：发送的数据包参数
**                      type：类型
**                      pParm：参数指针
**                      parmLen：参数长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t UpdataBase_PacketEncode(PUpdataBase_Packet pPacket, uint8_t type, uint8_t *pParm, uint16_t parmLen)
{
    pPacket->type = type;
    pPacket->err = UpdataBase_NOERR;
    pPacket->parmLen = parmLen;
    if (parmLen != 0)
    {
        memcpy(pPacket->pParm, pParm, parmLen);
    }
    return UpdataBase_PACKETHEADSIZE + parmLen;
}

/*************************************************************
** Function name:       UpdataBase_InfoCRC
** Descriptions:        计算信息区域的CRC，算法为CRC16_CCITT_FALSE计算
** Input parameters:    pData：校验的数据
**                      length：长度
** Output parameters:   none
** Returned             返回crc计算结果
** Remarks:             None
*************************************************************/
uint16_t UpdataBase_InfoCRC(uint8_t *pData, uint16_t length)
{
    uint16_t initValue = 0xFFFF;
    uint16_t wCPoly = 0x1021;
    uint8_t wChar = 0;
    while (length--)
    {
        wChar = *(pData++);
        initValue ^= (wChar << 8);
        for (uint8_t cir = 0; cir < 8; cir++)
        {
            if (initValue & 0x8000)
                initValue = (initValue << 1) ^ wCPoly;
            else
                initValue = initValue << 1;
        }
    }
    return (initValue);
}

/*************************************************************
** Function name:       UpdataBase_InfoVerify
** Descriptions:        信息区域校验,并写入校验结果
** Input parameters:    pInfo：信息参数指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataBase_InfoVerify(PUpdataBase_FirmInfo pInfo)
{
    pInfo->infoCRC = UpdataBase_InfoCRC((uint8_t *)(&pInfo->infoCRC) + 2, sizeof(UpdataBase_FirmInfo) - 2);
}

/*************************************************************
** Function name:       UpdataBase_InfoCheck
** Descriptions:        固件信息区域检查
** Input parameters:    pInfo：信息参数指针
** Output parameters:   None
** Returned value:      true：固件信息正确，false，信息错误
** Remarks:             None
*************************************************************/
bool UpdataBase_InfoCheck(PUpdataBase_FirmInfo pInfo)
{
    uint16_t crc = 0;
    crc = UpdataBase_InfoCRC((uint8_t *)(&pInfo->infoCRC) + 2, sizeof(UpdataBase_FirmInfo) - 2);
    return crc == pInfo->infoCRC;
}

/*************************************************************
** Function name:       UpdataBase_GetCodeAllNum
** Descriptions:        获取源码数据包传输的总数
** Input parameters:    firmSize:固件大小
**                      codeSize：源码包大小
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t UpdataBase_GetCodeAllNum(uint32_t firmSize, uint16_t codeSize)
{
    uint16_t CodeAllNum = 0;
    CodeAllNum = firmSize / codeSize;
    if (firmSize % codeSize != 0)
    {
        CodeAllNum++;
    }
    return CodeAllNum;
}

/*************************************************************
** Function name:       UpdataBoot_CodeIsWrite
** Descriptions:        判断是否写入源码
** Input parameters:    pParm：固件升级参数指针
**                      firmwareAddress：固件文件的地址
** Output parameters:   None
** Returned value:      true，写入，false，未写入，同时置位写入标记
** Remarks:             None
*************************************************************/
uint8_t UpdataBase_CodeTableIsWrite(uint8_t *pTable, uint16_t codeNum)
{
    uint16_t index = codeNum / 8;
    uint8_t offset = codeNum % 8;
    if ((pTable[index] & (1 << offset)) != 0)
    {
        return true;
    }
    pTable[index] |= (1 << offset);
    return false;
}

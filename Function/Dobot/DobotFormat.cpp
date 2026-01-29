/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotFormat.c
** Latest modified date:    2019-09-19
** Latest version:          V1.0.0
** Description:             协议格式解析
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2019-09-19
** Version:                 V1.0.0
** Descriptions:            协议格式解析
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "DobotFormat.h"
#include <stddef.h>
#include <string.h>
#include "stdio.h"

/*************************************************************
    Dobot 参数编码
*************************************************************/

/*************************************************************
** Function name:       DobotFormat_SumEncoder
** Descriptions:        协议和校验
** Input parameters:    pPacket：数据包指针，Payload部分
**                      packetLength:数据包长度，Payload部分
** Output parameters:   no
** min：                no
** Returned             二补数
*************************************************************/
uint8_t DobotFormat_SumEncoder(uint8_t *pPacket, uint16_t packetLength)
{
    uint8_t sum = 0;
    uint16_t cir = 0;
    for (cir = 0; cir < packetLength; cir++)
    {
        sum += pPacket[cir];
    }
    sum = 256 - sum;
    return sum;
}

/*************************************************************
** Function name:       DobotFormat_PacketEncoder
** Descriptions:        数据包编码
** Input parameters:    pParm：数据包参数指针
**                      pDataLen:pData的数据长度
** Output parameters:   no
** min：                no
** Returned             数据包总长度
*************************************************************/
uint16_t DobotFormat_PacketEncoder(PDobotFormatPARM pParm, uint8_t pDataLen)
{
    pParm->sof1 = DobotFormat_SOF;
    pParm->sof2 = DobotFormat_SOF;
    pParm->length = pDataLen + 2;
    pParm->pdata[pDataLen] = DobotFormat_SumEncoder(&pParm->id, pParm->length);
    return pDataLen + 6;
}

/*************************************************************
    END Dobot 参数编码
*************************************************************/

/*************************************************************
    Dobot 错误代码
*************************************************************/

/*************************************************************
** Function name:       ErrCodeRead
** Descriptions:        读取指令的错误代码
** Input parameters:    pPacketParm：数据包参数指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void DobotFormat_ErrCodeRead(PProtocolFrame_PacketPARM pPacketParm)
{
    pPacketParm->errCode = NULL;
}
/*************************************************************
** Function name:       ErrCodeSend
** Descriptions:        发送指令的错误代码
** Input parameters:    pPacketParm：数据包参数指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void DobotFormat_ErrCodeSend(PProtocolFrame_PacketPARM pPacketParm)
{
    return;
}

/*************************************************************
    END Dobot 错误代码
*************************************************************/

/*************************************************************
    Dobot 协议格式
*************************************************************/

/*************************************************************
** Function name:       DobotFormat_Sof2Check
** Descriptions:        前导符2解析
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             返回值：ProtocolFrame_DEFALSE
**                              ProtocolFrame_DETRUE
**                              ProtocolFrame_DENEXT
*************************************************************/
uint8_t DobotFormat_Sof2Check(uint8_t *pData, uint16_t length)
{
    if (pData[1] == DobotFormat_SOF)
    {
        return ProtocolFrame_DETRUE;
    }
    else
    {
        return ProtocolFrame_DEFALSE;
    }
}

/*************************************************************
** Function name:       DobotFormat_Sof2GetLength
** Descriptions:        前导符2步骤开始校验的长度
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             该步骤校验的长度
*************************************************************/
uint16_t DobotFormat_Sof2CheckLength(uint8_t *pData, uint16_t length)
{
    return 2;
}

/*************************************************************
** Function name:       DobotFormat_Sof2Check
** Descriptions:        包长校验
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             返回值：ProtocolFrame_DEFALSE
**                              ProtocolFrame_DETRUE
**                              ProtocolFrame_DENEXT
*************************************************************/
uint8_t DobotFormat_PackLengthCheck(uint8_t *pData, uint16_t length)
{
    if (pData[2] >= 2 && pData[2] < DobotFormat_PAYLOAD_LENGTH)
    {
        return ProtocolFrame_DETRUE;
    }
    else
    {
        return ProtocolFrame_DEFALSE;
    }
}

/*************************************************************
** Function name:       DobotFormat_PackLengthCheckLength
** Descriptions:        包长开始校验的长度
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             该步骤校验的长度
*************************************************************/
uint16_t DobotFormat_PackLengthCheckLength(uint8_t *pData, uint16_t length)
{
    return 3;
}

/*************************************************************
** Function name:       DobotFormat_CRCCheck
** Descriptions:        CRC校验
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             返回值：ProtocolFrame_DEFALSE
**                              ProtocolFrame_DETRUE
**                              ProtocolFrame_DENEXT
*************************************************************/
uint8_t DobotFormat_SumCheck(uint8_t *pData, uint16_t length)
{
    uint8_t packetLength = pData[2] + 3;
    uint8_t cir;
    uint8_t sum = 0;
    for (cir = 3; cir < packetLength; cir++)
    {
        sum += pData[cir];
    }
    sum += pData[packetLength];
    if (sum == 0)
    {
        return ProtocolFrame_DETRUE;
    }
    else
    {
        return ProtocolFrame_DEFALSE;
    }
}

/*************************************************************
** Function name:       DobotFormat_CRCCheckLength
** Descriptions:        开始CRC校验的长度
** Input parameters:    pData：当前接收到的所有数据
**                      length:当前数据包长度
** Output parameters:   no
** min：                no
** Returned             该步骤校验的长度
*************************************************************/
uint16_t DobotFormat_SumCheckLength(uint8_t *pData, uint16_t length)
{
    return pData[2] + 4;
}

/* 输出格式信息 */
ProtocolFrame_FORMAT_EXPORT(DobotFormat,
                            DobotFormat_ErrCodeRead, DobotFormat_ErrCodeSend,
                            DobotFormat_SOF,
                            DobotFormat_Sof2Check, DobotFormat_Sof2CheckLength,
                            DobotFormat_PackLengthCheck, DobotFormat_PackLengthCheckLength,
                            DobotFormat_SumCheck, DobotFormat_SumCheckLength, )

    /*************************************************************
    END Dobot 协议格式
*************************************************************/

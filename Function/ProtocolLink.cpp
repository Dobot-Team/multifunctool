/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolLink.cpp
** Latest modified date:    2020-04-23
** Latest version:          V1.0.0
** Description:             Dobot协议链路
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-23
** Version:                 V1.0.0
** Descriptions:            
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "ProtocolLink.h"
#include "DobotV3Format.h"
ProtocolClass gProtocolClass;

/*************************************************************
** Function name:       readData
** Descriptions:        读取数据
** Input parameters:    pData：存放读取数据的指针
**                      length:读取的数据长度
** Output parameters:   no
** min：                no
** Returned             返回读取的数据个数
*************************************************************/
uint16_t ProtocolLink_ReadData(uint8_t *pData, uint16_t length)
{
    return gProtocolClass.ComReadData(pData, length);
}

/*************************************************************
** Function name:       SendData
** Descriptions:        发送数据
** Input parameters:    pData：存放读取数据的指针
**                      length：发送的数据长度
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void ProtocolLink_SendData(uint8_t *pData, uint16_t length)
{
    gProtocolClass.WriteSendQueue(pData, length);
}

ProtocolFrame_LINK_EXPORT(ProtocolLink, 500, DobotV3Format_PACKET_LENGTH, ProtocolLink_ReadData, ProtocolLink_SendData)

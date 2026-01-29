/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolLink.h
** Latest modified date:    2020-04-23
** Latest version:          V1.0.0
** Description:             Dobow 协议链路
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-23
** Version:                 V1.0.0
** Descriptions:            Dobow 协议链路
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef ProtocolLink_h
#define ProtocolLink_h
#include "ProtocolClass.h"
extern "C"
{
#include "ProtocolFrame.h"
}

extern ProtocolClass gProtocolClass;

ProtocolFrame_LINK_EXTERN(ProtocolLink) extern void ProtocolLink_SendData(uint8_t *pData, uint16_t length);
extern uint16_t ProtocolLink_ReadData(uint8_t *pData, uint16_t length);
#endif /* ProtocolLink_h */

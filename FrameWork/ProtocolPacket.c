/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolPacket.c
** Latest modified date:    2019-12-14
** Latest version:          V1.0.0
** Description:             协议指令功能-指令发送
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Latest modified date:    2019-12-14
** Latest version:          V1.0.0
** Description:             协议指令功能-指令发送
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "ProtocolPacket.h"
#include <string.h>

/*************************************************************
** Function name:       ProtocolFCRout_PacketCallBack
** Descriptions:        解析到正确数据包的回调函数
** Input parameters:    pFuncStruct：应用功能结构指针
**                      pPacketParm：数据包参数指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void ProtocolPacket_PacketCallBack(PProtocolFrame_FuncSTRUCT pFuncStruct, PProtocolFrame_PacketPARM pPacketParm)
{
    PProtocolPacketPARM pParm = (PProtocolPacketPARM)pFuncStruct->pFuncParmPtr;
    pParm->GetPacket(pPacketParm->pPacket, pPacketParm->packetLength);
}

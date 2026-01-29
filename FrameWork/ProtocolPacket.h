/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolPacket.h
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
#ifndef ProtocolPacket_h
#define ProtocolPacket_h
#include "ProtocolFrame.h"

#pragma pack(1)
struct ProtocolPacketParm
{
    /*************************************************************
    ** Function name:       GetPacket
    ** Descriptions:        获取数据包回调函数
    **                      pPacket:数据包指针
    **                      dataLen：数据包长度
    ** Input parameters:    None
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*GetPacket)(uint8_t *pPacket, uint16_t dataLen);
};
typedef struct ProtocolPacketParm ProtocolPacketPARM;
typedef ProtocolPacketPARM *PProtocolPacketPARM;
#pragma pack()

/*************************************************************
** Function name:       ProtocolPacket_EXPORT
** Descriptions:        定义指令发送功能
** Input parameters:    funcName：(ProtocolFrame_FuncSTRUCT)功能名称
**                      GetPacket:获取数据包回调函数，具体查看 ProtocolPacketPARM
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolPacket_EXPORT(funcName, GetPacket)                    \
    ProtocolPacketPARM gProtocolPacketParm##funcName =                \
        {                                                             \
            GetPacket,                                                \
    };                                                                \
    ProtocolFrame_FUNC_EXPORT(funcName,                               \
                              (void *)&gProtocolPacketParm##funcName, \
                              ProtocolPacket_PacketCallBack,          \
                              NULL)

/*************************************************************
** Function name:       ProtocolPacket_EXTERN
** Descriptions:        声明指令发送功能
** Input parameters:    funcName：(ProtocolFrame_FuncSTRUCT)功能名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolPacket_EXTERN(funcName) \
    ProtocolFrame_FUNC_EXTERN(funcName)

extern void ProtocolPacket_PacketCallBack(PProtocolFrame_FuncSTRUCT pFuncStruct, PProtocolFrame_PacketPARM pPacketParm);

#endif /* ProtocolPacket_h */

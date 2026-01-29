/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolFrame.h
** Latest modified date:    2019-12-16
** Latest version:          V2.0.0
** Description:             自定义协议框架
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2019-09-19
** Version:                 V1.0.0
** Descriptions:            自定义协议框架
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/

#ifndef ProtocolFrame_h
#define ProtocolFrame_h

#include <stdint.h>
#include <stdbool.h>
#include "stddef.h"

/* 解析状态 */
#define ProtocolFrame_DEFALSE 0 /* 解析失败 */
#define ProtocolFrame_DETRUE 1  /* 解析成功 */
#define ProtocolFrame_DENEXT 2  /* 下一次解析 */

#pragma pack(1)

typedef struct ProtocolFrameParm ProtocolFramePARM; /* 协议框架参数结构体类型*/
typedef ProtocolFramePARM *PProtocolFramePARM;      /* 协议框架参数结构体指针类型*/

typedef struct ProtocolFrame_LinkParm ProtocolFrame_LinkPARM; /* 解析链路结构体类型 */
typedef ProtocolFrame_LinkPARM *PProtocolFrame_LinkPARM;      /* 解析链路结构体指针类型 */

typedef struct ProtocolFrame_FormatParm ProtocolFrame_FormatPARM; /* 解析的数据格式类型*/
typedef ProtocolFrame_FormatPARM *PProtocolFrame_FormatPARM;      /* 解析的数据格式指针类型*/

typedef struct ProtocolFrame_PacketParm ProtocolFrame_PacketPARM; /* 传递给用户的参数结构体类型 */
typedef ProtocolFrame_PacketPARM *PProtocolFrame_PacketPARM;      /* 传递给用户的参数结构体指针类型 */

/* 数据包数据 */
struct ProtocolFrame_PacketParm
{
    PProtocolFrame_LinkPARM pLink; /* 链路指针 */
    uint8_t *pPacket;              /* 数据包指针 */
    uint16_t packetLength;         /* 数据包长度 */
    uint8_t errCode;               /* 错误代码 */
    uint8_t sendMark : 1;          /* 数据发送标记，true发送，false不发送 */
    uint8_t exitCallPack : 1;      /* 退出回调函数，true退出，false不退出 */
};

/* 解析步骤结构体 */
struct ProtocolFrame_StepParm
{
    /*************************************************************
    ** Function name:       StepCheck
    ** Descriptions:        步骤校验，在接收到的数据个数大于步骤长度的时候调用
    ** Input parameters:    pData：当前接收到的所有数据
    **                      length:当前数据包长度
    ** Output parameters:   no
    ** min：                no
    ** Returned             返回值：ProtocolFrame_DEFALSE 校验失败
    **                              ProtocolFrame_DETRUE 校验成功
    *************************************************************/
    uint8_t (*StepCheck)(uint8_t *pData, uint16_t length); /* 解析步骤校验 */
    /*************************************************************
    ** Function name:       CheckLength
    ** Descriptions:        获取该步骤开始校验的长度
    ** Input parameters:    pData：当前接收到的所有数据
    **                      length:当前数据包长度
    ** Output parameters:   no
    ** min：                no
    ** Returned             该步骤校验的长度
    *************************************************************/
    uint16_t (*CheckLength)(uint8_t *pData, uint16_t length); /* 获取该步骤开始校验的长度 */
};
typedef struct ProtocolFrame_StepParm ProtocolFrame_StepPARM; /* 解析步骤结构体类型 */
typedef ProtocolFrame_StepPARM *PProtocolFrame_StepPARM;      /* 解析步骤结构体指针类型 */

/* 解析的数据格式 */
struct ProtocolFrame_FormatParm
{
    /*************************************************************
    ** Function name:       ErrCodeRead
    ** Descriptions:        读取指令的错误代码
    ** Input parameters:    pPacketParm：数据包参数指针
    ** Output parameters:   no
    ** min：                no
    ** Returned             no
    *************************************************************/
    void (*ErrCodeRead)(PProtocolFrame_PacketPARM pPacketParm);
    /*************************************************************
    ** Function name:       ErrCodeSend
    ** Descriptions:        发送指令的错误代码
    ** Input parameters:    pPacketParm：数据包参数指针
    ** Output parameters:   no
    ** min：                no
    ** Returned             no
    *************************************************************/
    void (*ErrCodeSend)(PProtocolFrame_PacketPARM pPacketParm);
    uint8_t sof;                       /* 前导符 */
    PProtocolFrame_StepPARM pStepList; /* 解析步骤指针 */
    uint16_t stepListSize;             /* 解析步骤个数 */
};

typedef struct ProtocolFrame_FuncStruct ProtocolFrame_FuncSTRUCT; /* 应用功能结构体类型 */
typedef ProtocolFrame_FuncSTRUCT *PProtocolFrame_FuncSTRUCT;      /* 应用功能结构体指针类型 */
/* 应用功能结构 */
struct ProtocolFrame_FuncStruct
{
    /*************************************************************
    ** Function name:       PacketCallBack
    ** Descriptions:        解析到正确数据包的回调函数
    ** Input parameters:    pFuncStruct：应用功能结构指针
    **                      pPacketParm：数据包参数指针
    ** Output parameters:   no
    ** min：                no
    ** Returned             no
    *************************************************************/
    void (*PacketCallBack)(PProtocolFrame_FuncSTRUCT pFuncStruct, PProtocolFrame_PacketPARM pPacketParm);
    /*************************************************************
    ** Function name:       Loop
    ** Descriptions:        循环执行函数
    ** Input parameters:    pFuncStruct：应用功能结构指针
    **                      pPacketParm：数据包参数指针
    ** Output parameters:   no
    ** min：                no
    ** Returned             no
    *************************************************************/
    void (*Loop)(PProtocolFrame_FuncSTRUCT pFuncStruct, PProtocolFrame_PacketPARM pPacketParm);
    void *pFuncParmPtr; /* 功能参数指针 */
};

/* 应用功能参数 */
struct ProtocolFrame_FuncParm
{
    uint8_t loopMark : 1;
};
typedef struct ProtocolFrame_FuncParm ProtocolFrame_FuncPARM; /* 应用功能结构体类型 */
typedef ProtocolFrame_FuncPARM *PProtocolFrame_FuncPARM;      /* 应用功能结构体指针类型 */

/* 应用功能列表 */
struct ProtocolFrame_FuncList
{
    PProtocolFrame_FuncSTRUCT *pFuncStructList;
    PProtocolFrame_FuncPARM pFuncParmList;
    uint8_t funcListSize;
};
typedef struct ProtocolFrame_FuncList ProtocolFrame_FuncLIST; /* 应用功能列表结构体类型 */
typedef ProtocolFrame_FuncLIST *PProtocolFrame_FuncLIST;      /* 应用功能列表结构体指针类型 */

/* 解析链路 */
struct ProtocolFrame_LinkParm
{
    uint16_t deTimeOut;  /* 解包超时时间 */
    uint8_t *pDeMsgBuff; /* 解析消息buf */
    uint32_t maxMsgNum;  /* 最大消息个数 */
    /*************************************************************
    ** Function name:       readData
    ** Descriptions:        读取数据
    ** Input parameters:    pData：存放读取数据的指针
    **                      length:读取的数据长度
    ** Output parameters:   no
    ** min：                no
    ** Returned             返回读取的数据个数
    *************************************************************/
    uint16_t (*readData)(uint8_t *pData, uint16_t length); /* 读取数据函数 */
    /*************************************************************
    ** Function name:       SendData
    ** Descriptions:        发送数据
    ** Input parameters:    pData：存放读取数据的指针
    **                      length：发送的数据长度
    ** Output parameters:   no
    ** min：                no
    ** Returned             no
    *************************************************************/
    void (*SendData)(uint8_t *pData, uint16_t length); /* 读取数据函数 */

    uint8_t enable : 1;  /* 链路使能 */
    uint8_t sofMark : 1; /* 前导符解析标记,false未解析到，true已完成前导符解析 */

    PProtocolFrame_FormatPARM pFormat; /* 数据格式 */
    PProtocolFrame_FuncLIST pFuncList; /* 功能列表 */

    uint32_t msgReadIndex;  /* 读取的消息索引 */
    uint32_t msgWriteIndex; /* 写入的消息索引 */
    uint8_t deStep;         /* 当前解析步骤 */
    uint16_t deLenght;      /* 当前解析长度 */
    uint16_t deStepLength;  /* 步骤的解析长度 */
    uint32_t deStartTime;   /* 解包开始时间 */
};

/* 框架参数结构体 */
struct ProtocolFrameParm
{
    PProtocolFrame_LinkPARM *pLinkList; /* 解析链路表 */
    uint8_t linkMaxNum;                 /* 解析链路最大个数 */
    uint8_t linkNum;                    /* 已注册解析链路个数 */

    uint8_t *pPacket;                      /* 数据包指针 */
    PProtocolFrame_PacketPARM pPacketParm; /* 传递给用户的参数 */
};

#pragma pack()

extern uint32_t gProtocolFrame_TimeCNT; /* 时间计数 */

/*************************************************************
** Function name:       ProtocolFrame_TimeBase
** Descriptions:        时基,放在周期为1ms的函数里面执行
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_TimeBase() \
    gProtocolFrame_TimeCNT++

/*************************************************************
** Function name:       ProtocolFrame_GETSTARTTIME
** Descriptions:        获取起始时间
** Input parameters:    None
** Output parameters:   None
** Returned value:      (uint32_t)起始时间
*************************************************************/
#define ProtocolFrame_GETSTARTTIME(void) \
    gProtocolFrame_TimeCNT

/*************************************************************
** Function name:       ProtocolFrame_TIMEOUT
** Descriptions:        检查超时
** Input parameters:    timeOut：(uint32_t)超时时间
**                      startTime:(uint32_t)开始的时间
** Output parameters:   None
** Returned value:      false,未超时，true，超时
*************************************************************/
#define ProtocolFrame_TIMEOUT(timeOut, startTime) \
    ((gProtocolFrame_TimeCNT - startTime) > timeOut ? true : false)

/*************************************************************
** Function name:       ProtocolFrame_ELAPSETIME
** Descriptions:        获取距离起始时经过的时间
** Input parameters:    startTime:(uint32_t)开始的时间
** Output parameters:   None
** Returned value:      (uint32_t)经过的时间
*************************************************************/
#define ProtocolFrame_ELAPSETIME(startTime) \
    (gProtocolFrame_TimeCNT - startTime)

/*************************************************************
** Function name:       ProtocolFrame_FRAME_EXPORT
** Descriptions:        定义一个框架
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
**                      packetMaxSize:(uint16_t)数据包最大长度
**                      linkMaxNum：（uint8_t）可以容纳的最大链路个数
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_FRAME_EXPORT(frameName, packetMaxSize, linkMaxNum)    \
    uint8_t gProtocolFrame_Packet##frameName[packetMaxSize];                \
    PProtocolFrame_LinkPARM gProtocolFrame_LinkList##frameName[linkMaxNum]; \
    ProtocolFrame_PacketPARM gProtocolFrame_packetParm##frameName;          \
    ProtocolFramePARM frameName =                                           \
        {                                                                   \
            gProtocolFrame_LinkList##frameName,                             \
            linkMaxNum,                                                     \
            0,                                                              \
            gProtocolFrame_Packet##frameName,                               \
            &gProtocolFrame_packetParm##frameName,                          \
    };

/*************************************************************
** Function name:       ProtocolFrame_FRAME_EXTERN
** Descriptions:        声明框架
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_FRAME_EXTERN(frameName) \
    extern ProtocolFramePARM frameName;

/*************************************************************
** Function name:       ProtocolFrame_LINK_EXPORT
** Descriptions:        定义链路
** Input parameters:    linkName：(ProtocolFrame_LinkPARM)链路名称
**                      packetMaxSize:(uint16_t)数据包最大长度
**                      linkMaxNum：（uint8_t）可以容纳的最大链路个数
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_LINK_EXPORT(linkName, timeOut, packetMaxSize, ReadData, SendData) \
    uint8_t gProtocolFrame_LinkDeMsgBuf##linkName[packetMaxSize];                       \
    ProtocolFrame_LinkPARM linkName = {                                                 \
        timeOut,                                                                        \
        gProtocolFrame_LinkDeMsgBuf##linkName,                                          \
        packetMaxSize,                                                                  \
        ReadData,                                                                       \
        SendData,                                                                       \
        true,                                                                           \
        false,                                                                          \
    };

/*************************************************************
** Function name:       ProtocolFrame_LINK_EXTERN
** Descriptions:        声明链路
** Input parameters:    linkName：(ProtocolFrame_LinkPARM)链路名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_LINK_EXTERN(linkName) \
    extern ProtocolFrame_LinkPARM linkName;

/*************************************************************
** Function name:       ProtocolFrame_LINK_EXTERN
** Descriptions:        仅使能 协议框架 指定链路
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
**                      linkName：(ProtocolFrame_LinkPARM)使能链路名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_FRAMELINK_ENBALE_ONLY(FrameName, linkName) \
    ProtocolFrame_FrameLinkEnableOnly(&FrameName, &linkName)

/*************************************************************
** Function name:       ProtocolFrame_LINK_EXTERN
** Descriptions:        仅禁能 协议框架 指定链路
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
**                      linkName：(ProtocolFrame_LinkPARM)禁能链路名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_FRAMELINK_DISABLE_ONLY(FrameName, linkName) \
    ProtocolFrame_FrameLinkDisableOnly(&FrameName, &linkName)

/*************************************************************
** Function name:       ProtocolFrame_LINK_ENABLE
** Descriptions:        使能链路
** Input parameters:    linkName：(ProtocolFrame_LinkPARM)链路名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_LINK_ENABLE(linkName) \
    linkName.enable = true

/*************************************************************
** Function name:       ProtocolFrame_LINK_DISABLE
** Descriptions:        禁能链路
** Input parameters:    linkName：(ProtocolFrame_LinkPARM)链路名称
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_LINK_DISABLE(linkName) \
    linkName.enable = false

/*************************************************************
** Function name:       ProtocolFrame_GET_LINK_STATUS
** Descriptions:        获取链路状态
** Input parameters:    linkName：(ProtocolFrame_LinkPARM)链路名称
** Output parameters:   None
** Returned value:      true，使能，false禁能
*************************************************************/
#define ProtocolFrame_GET_LINK_STATUS(linkName) \
    linkName.enable

/*************************************************************
** Function name:       ProtocolFrame_FORMAT_EXPORT
** Descriptions:        定义解析格式（NOFUNC）
** Input parameters:    formatName：(ProtocolFrame_FormatPARM)格式名称
**                      ErrCodeRead：错误代码读取回调函数
**                      ErrCodeSend：错误代码发送回调函数
**                      sof：(uint8_t)前导符
**                      deStep...：解析步骤
**                          CheckLength：校验长度获取回调函数
**                          StepCheck：数据校验回调函数
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_FORMAT_EXPORT(formatName,                   \
                                    ErrCodeRead, ErrCodeSend,     \
                                    sof, deStep...)               \
    ProtocolFrame_StepPARM gProtocol_FormatDeStep##formatName[] = \
        {                                                         \
            deStep};                                              \
    ProtocolFrame_FormatPARM formatName =                         \
        {                                                         \
            ErrCodeRead,                                          \
            ErrCodeSend,                                          \
            sof,                                                  \
            gProtocol_FormatDeStep##formatName,                   \
            sizeof(gProtocol_FormatDeStep##formatName) / sizeof(ProtocolFrame_StepPARM)};

/*************************************************************
** Function name:       ProtocolFrame_FORMAT_EXTERN
** Descriptions:        声明解析格式
** Input parameters:    formatName：(ProtocolFrame_FormatPARM)格式名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_FORMAT_EXTERN(formatName) \
    extern ProtocolFrame_FormatPARM formatName;

/*************************************************************
** Function name:       ProtocolFrame_FUNC_EXPORT
** Descriptions:        定义功能
** Input parameters:    funcName：(ProtocolFrame_FuncSTRUCT)功能名称
**                      pFuncParmPtr：(void*)用户参数指针
**                      PacketCallBack：数据包回调函数
**                      Loop：循环执行回调函数
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_FUNC_EXPORT(funcName, pFuncParmPtr, PacketCallBack, Loop) \
    ProtocolFrame_FuncSTRUCT funcName =                                         \
        {                                                                       \
            PacketCallBack,                                                     \
            Loop,                                                               \
            pFuncParmPtr,                                                       \
    };

/*************************************************************
** Function name:       ProtocolFrame_FUNC_EXTERN
** Descriptions:        声明功能
** Input parameters:    funcName：(ProtocolFrame_FuncSTRUCT)功能名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_FUNC_EXTERN(funcName) \
    extern ProtocolFrame_FuncSTRUCT funcName;

/*************************************************************
** Function name:       ProtocolFrame_FUNCLIST_EXPORT
** Descriptions:        定义功能列表
** Input parameters:    funcListName：(ProtocolFrame_FuncLIST)功能列表名称
**                      pFuncName...：(PProtocolFrame_FuncSTRUCT)功能指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_FUNCLIST_EXPORT(funcListName, pFuncName...)                                    \
    PProtocolFrame_FuncSTRUCT gProtocolFrame_FuncStructList##funcListName[] =                        \
        {                                                                                            \
            pFuncName};                                                                              \
    ProtocolFrame_FuncPARM gProtocolFrame_FuncStrucParm##funcListName                                \
        [sizeof(gProtocolFrame_FuncStructList##funcListName) / sizeof(PProtocolFrame_FuncSTRUCT)];   \
    ProtocolFrame_FuncLIST funcListName =                                                            \
        {                                                                                            \
            gProtocolFrame_FuncStructList##funcListName,                                             \
            gProtocolFrame_FuncStrucParm##funcListName,                                              \
            sizeof(gProtocolFrame_FuncStructList##funcListName) / sizeof(PProtocolFrame_FuncSTRUCT), \
    };

/*************************************************************
** Function name:       ProtocolFrame_FUNCLIST_EXTERN
** Descriptions:        声明功能列表
** Input parameters:    funcListName：(ProtocolFrame_FuncLIST)功能列表名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_FUNCLIST_EXTERN(funcListName) \
    extern ProtocolFrame_FuncLIST funcListName;

/*************************************************************
** Function name:       ProtocolFrame_EXEC
** Descriptions:        协议执行
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_EXEC(frameName) \
    ProtocolFrame_exec(&frameName)

/*************************************************************
** Function name:       ProtocolFrame_LINK_FORMAT_FUNCLIST
** Descriptions:        链接框架，链路，格式
** Input parameters:    frameName：(ProtocolFramePARM)框架名称
**                      linkName：(ProtocolFrame_LinkPARM)链路名称
**                      formatName：(ProtocolFrame_FormatPARM)格式名称
**                      funcListName：(ProtocolFrame_FuncLIST)功能列表名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolFrame_LINK_FORMAT_FUNCLIST(frameName, linkName, formatName, funcListName) \
    ProtocolFrame_Link_Format_FuncList(&frameName, &linkName, &formatName, &funcListName)

#ifdef __cplusplus
extern "C"
{
#endif
    extern void ProtocolFrame_Link_Format_FuncList(PProtocolFramePARM pFrame,
                                                   PProtocolFrame_LinkPARM pLink,
                                                   PProtocolFrame_FormatPARM pFormat,
                                                   PProtocolFrame_FuncLIST pFuncList);
    extern void ProtocolFrame_exec(PProtocolFramePARM pFrame);
    extern void ProtocolFrame_FrameLinkEnableOnly(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink);
    extern void ProtocolFrame_FrameLinkDisableOnly(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink);

#ifdef __cplusplus
}
#endif
#endif /* ProtocolFrame_h */

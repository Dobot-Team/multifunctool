/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataMaster.h
** Latest modified date:    2020-04-25
** Latest version:          V1.0.0
** Description:             固件升级主机
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-25
** Version:                 V1.0.0
** Descriptions:            固件升级主机
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef UpdataMaster_h
#define UpdataMaster_h
#include "UpdataBase.h"
#include "stdint.h"
#include "stddef.h"

#define UpdataMaster_VERIFYCACHE 64

#define UpdataMaster_FIRMMAXSIZE 1024 /* 1M的固件大小 */
#define UpdataMaster_PACKETMIN 200    /* 数据包最小大小 */
#define UpdataBase_TABLESIZE (UpdataMaster_FIRMMAXSIZE * 1024 / (UpdataMaster_PACKETMIN - (UpdataBase_PACKETSIZE - UpdataBase_CODEPACSIZE)) / 8)

#pragma pack(1)

typedef enum
{
    UpdataMaster_FAIL = 0,
    UpdataMaster_SUCCESS,
    UpdataMaster_SENDING,
    UpdataMaster_NOACK,
} UpdataMaster_SendStatus;

typedef struct SUpdataMasterParm UpdataMasterParm; /* UpdataMasterParm 类型 */
typedef UpdataMasterParm *PUpdataMasterParm;       /* PUpdataMasterParm 指针类型 */

struct SUpdataMasterLink
{
    /*************************************************************
    ** Function name:       SendData
    ** Descriptions:        发送数据
    ** Input parameters:    pData：需要发送的数据指针
    **                      len：需要发送的数据长度
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*SendData)(uint8_t *pData, uint16_t len);
};
typedef struct SUpdataMasterLink UpdataMasterLink; /* UpdataMasterLink 类型 */
typedef UpdataMasterLink *PUpdataMasterLink;       /* PUpdataMasterLink 指针类型 */

struct SUpdataBase_File
{
    /*************************************************************
    ** Function name:       Open
    ** Descriptions:        打开文件
    ** Input parameters:    fileAddr：打开文件的地址
    ** Output parameters:   None
    ** Returned value:      true，打开成功，false，打开失败
    ** Remarks:             None
    *************************************************************/
    bool (*Open)(char *pFileAddr);
    /*************************************************************
    ** Function name:       Close
    ** Descriptions:        关闭文件
    ** Input parameters:    None
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*Close)(void);
    /*************************************************************
    ** Function name:       Read
    ** Descriptions:        读取文件
    ** Input parameters:    Addr：读取文件的地址
    **                      pData:读取的数据指针
    **                      dataLen：读取的数据长度
    ** Output parameters:   None
    ** Returned value:      返回读取到的数据长度
    ** Remarks:             None
    *************************************************************/
    uint16_t (*Read)(uint32_t Addr, uint8_t *pData, uint16_t dataLen);
    /*************************************************************
    ** Function name:       GetSize
    ** Descriptions:        获取文件大小
    ** Input parameters:    None
    ** Output parameters:   None
    ** Returned value:      文件大小
    ** Remarks:             None
    *************************************************************/
    uint32_t (*GetSize)(void);
    char *pFileAddr;
    uint16_t fileAddrMax;
    uint8_t verifyCache[UpdataMaster_VERIFYCACHE];
};
typedef struct SUpdataBase_File UpdataBase_File; /* UpdataBase_File 类型 */
typedef UpdataBase_File *PUpdataBase_File;       /* PUpdataBase_File 指针类型 */

/* 回调函数集合 */
struct SUpdataMasterCB
{
    /*************************************************************
    ** Function name:       DevInfoCB
    ** Descriptions:        设备信息回调函数
    ** Input parameters:    pInfo：固件信息指针,为NULL表示固件信息为空
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*DevInfoCB)(PUpdataBase_FirmInfo pInfo);
    /*************************************************************
    ** Function name:       UpdataFinishCB
    ** Descriptions:        更新结束回调
    ** Input parameters:    pInfo：更新完成后的固件信息指针
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*UpdataFinishCB)(PUpdataBase_FirmInfo pInfo);
    /*************************************************************
    ** Function name:       UpdataErrCB
    ** Descriptions:        更新错误回调
    ** Input parameters:    updataStep：更新的步骤
    **                      errType：错误类型
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*UpdataErrCB)(uint8_t updataStep, uint8_t errType);
    /*************************************************************
    ** Function name:       ScheduleChangeCB
    ** Descriptions:        更新进度变化
    ** Input parameters:    pc:当前进度百分比
    **                      info：当前更新进度信息
    ** Output parameters:   None
    ** Returned value:      None
    ** Remarks:             None
    *************************************************************/
    void (*ScheduleChangeCB)(float pc, char *info);
};
typedef struct SUpdataMasterCB UpdataMasterCB; /* UpdataMasterCB 类型 */
typedef UpdataMasterCB *PUpdataMasterCB;       /* PUpdataMasterCB 指针类型 */

struct SUpdataMasterExecParm
{
    uint16_t transKey;                         /* 传输秘钥 */
    uint16_t transCodeSize;                    /* 一次传输的源码大小 */
    uint16_t codeAllNum;                       /* 数据包总数 */
    uint16_t curCodeNum;                       /* 当前数据包数 */
    uint8_t updataTable[UpdataBase_TABLESIZE]; /* 更新进度表缓存 */
    uint32_t sendTimeStart[UpdataBase_MAX];    /* 指令发送开始时间 */
    bool cmdSendMark[UpdataBase_MAX];          /* 指令发送标记 */
    bool ackMark[UpdataBase_MAX];              /* 指令响应标记 */
};
typedef struct SUpdataMasterExecParm UpdataMasterExecParm; /* UpdataMasterExecParm 类型 */
typedef UpdataMasterExecParm *PUpdataMasterExecParm;       /* PUpdataMasterExecParm 指针类型 */

struct SUpdataMasterHandle
{
    uint8_t type;
    void (*Handle)(PUpdataMasterParm p, PUpdataBase_Packet pRead);
};
typedef struct SUpdataMasterHandle UpdataMasterHandle; /* UpdataMasterHandle 类型 */
typedef UpdataMasterHandle *PUpdataMasterHandle;       /* PUpdataMasterHandle 指针类型 */

struct SUpdataMasterSchedule
{
    bool updataMark;               /* true 正在更新，false未更新 */
    uint8_t updataStep;            /* 当前更新步骤 */
    float pcAll;                   /* 更新进度百分比 */
    uint8_t pcOne[UpdataBase_MAX]; /* 单个的更新进度 */
};
typedef struct SUpdataMasterSchedule UpdataMasterSchedule; /* UpdataMasterSchedule 类型 */
typedef UpdataMasterSchedule *PUpdataMasterSchedule;       /* PUpdataMasterSchedule 指针类型 */

struct SUpdataMasterParm
{
    PUpdataMasterCB CB;            /* 回调函数集合 */
    PUpdataMasterLink pLink;       /* 链路参数 */
    PUpdataBase_File file;         /* 文件操作参数 */
    uint16_t *pCmdTime;            /* 指令发送时间间隔 */
    uint8_t *pPacketCache;         /* 数据包缓存 */
    UpdataMasterSchedule schedule; /* 更新进度 */
    UpdataMasterExecParm execParm; /* 执行相关参数 */
    UpdataBase_FirmInfo infoCache; /* 固件信息缓存 */
};

#pragma pack()

/*************************************************************
** Function name:       UpdataMaster_CB_EXPORT
** Descriptions:        定义回调函数参数
** Input parameters:    name：（UpdataMasterCB）需符合变量命名规范
**                      DevInfoCB：设备信息回调函数，具体查看 UpdataMasterCB
**                      UpdataFinishCB：更新结束回调函数，具体查看 UpdataMasterCB
**                      UpdataErrCB：更新错误回调函数，具体查看 UpdataMasterCB
**                      UpdataStepChangeCB：更新步骤改变回调函数，具体查看 UpdataMasterCB
**                      ScheduleChangeCB：更新进度变化回调函数，具体查看 UpdataMasterCB
**                      FuncErrCB：函数错误回调函数，具体查看 UpdataMasterCB
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_CB_EXPORT(name,                          \
                               DevInfoCB, UpdataFinishCB,     \
                               UpdataErrCB, ScheduleChangeCB) \
    UpdataMasterCB name = {                                   \
        DevInfoCB,                                            \
        UpdataFinishCB,                                       \
        UpdataErrCB,                                          \
        ScheduleChangeCB,                                     \
    };

/*************************************************************
** Function name:       UpdataMaster_CB_EXTERN
** Descriptions:        声明回调函数参数
** Input parameters:    name：（UpdataMasterCB）需符合变量命名规范
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_CB_EXTERN(name) \
    extern UpdataMasterCB name;

/*************************************************************
** Function name:       UpdataMaster_FILE_EXPORT
** Descriptions:        定义文件相关参数
** Input parameters:    name：(UpdataBase_File) 需符合变量命名规范
**                      fileAddrMax：文件路径的最大大小
**                      Open:文件打开函数，具体查看 UpdataBase_File
**                      Close：文件关闭函数，具体查看 UpdataBase_File
**                      Read：文件读取函数，具体查看 UpdataBase_File
**                      GetSize：获取文件大小函数，具体查看 UpdataBase_File
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_FILE_EXPORT(name, fileAddrMax,          \
                                 Open, Close, Read, GetSize) \
    char gUpdataMaster_FileAddr##name[fileAddrMax];          \
    UpdataBase_File name =                                   \
        {                                                    \
            Open,                                            \
            Close,                                           \
            Read,                                            \
            GetSize,                                         \
            gUpdataMaster_FileAddr##name,                    \
            fileAddrMax,                                     \
    };

/*************************************************************
** Function name:       UpdataMaster_FILE_EXTERN
** Descriptions:        声明文件相关参数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_FILE_EXTERN(name) \
    extern UpdataBase_File name;

/*************************************************************
** Function name:       UpdataMaster_EXPORT
** Descriptions:        定义固件更新参数
** Input parameters:    name：（UpdataMasterParm）需符合变量命名规范
**                      packetMax：(uint16_t)数据包最大大小
**                      infoTime:(uint16_t) 获取信息指令的发送的时间间隔
**                      prepareTime:(uint16_t) 准备升级指令的发送的时间间隔
**                      isCompleteTime:(uint16_t) 验证代码是否完整的指令发送的时间间隔
**                      jumpAppTime:(uint16_t) 跳转到APP指令发送的时间间隔
**                      SendData：数据发送函数，具体查看 UpdataMasterLink
**                      File：（UpdataBase_File）文件相关参数
**                      CB：（UpdataMasterCB）回调函数参数
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_EXPORT(name, packetMax,                                    \
                            infoTime, prepareTime, isCompleteTime, jumpAppTime, \
                            SendData, File, CB)                                 \
    uint16_t gUpdataMaster_CmdTime##name[UpdataBase_MAX] =                      \
        {                                                                       \
            infoTime,                                                           \
            prepareTime,                                                        \
            0,                                                                  \
            isCompleteTime,                                                     \
            jumpAppTime,                                                        \
    };                                                                          \
    UpdataMasterLink gUpdataMasterLink##name =                                  \
        {                                                                       \
            SendData};                                                          \
    uint8_t gUpdataMaster_PacketCache##name[packetMax];                         \
    UpdataMasterParm name =                                                     \
        {                                                                       \
            &CB,                                                                \
            &gUpdataMasterLink##name,                                           \
            &File,                                                              \
            gUpdataMaster_CmdTime##name,                                        \
            gUpdataMaster_PacketCache##name,                                    \
    };

/*************************************************************
** Function name:       UpdataMaster_EXTERN
** Descriptions:        声明固件更新参数
** Input parameters:    name：（UpdataMasterParm）需符合变量命名规范
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_EXTERN(name) \
    extern UpdataMasterParm name;

extern uint8_t UpdataMaster_FirmVerify(PUpdataMasterParm pParm);
extern uint8_t UpdataMaster_GetDevInfo(PUpdataMasterParm pParm);
extern void UpdataMaster_UpdataStart(PUpdataMasterParm pParm, char *pFileAddr);
extern void UpdataMaster_UpdataFinish(PUpdataMasterParm pParm);
extern void UpdataMaster_UpdataHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead);
extern void UpdataMaster_UpdataExec(PUpdataMasterParm p);
#endif /* UpdataMaster_h */

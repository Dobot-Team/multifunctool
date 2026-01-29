/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataBase.h
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
#ifndef UpdataBase_h
#define UpdataBase_h
#include "stdint.h"
#include "stdbool.h"

#define UpdataBase_PACKETSIZE 1024                                                    /* 数据包大小，Type + err + parmLen + parm*/
#define UpdataBase_PACKETHEADSIZE 4                                                   /* 数据包头部信息大小，Type + err + parmLen */
#define UpdataBase_PACKETPARMSIZE (UpdataBase_PACKETSIZE - UpdataBase_PACKETHEADSIZE) /* 数据包参数段大小，parm */
#define UpdataBase_CODEPACSIZE (UpdataBase_PACKETPARMSIZE - 4)                        /* 传输源码数据包的时候源码大小 */
#define UpdataBase_PROVERIFYLEN 16                                                    /* 校验长度 */
/* 备注：
 *      从机：如果某些存储区域存在字节对齐的要求，则 UpdataBase_CODEPACSIZE 的大小必须为字节对齐的整数倍 
 *      主机：UpdataBase_PACKETSIZE的大小必须大于从机的UpdataBase_PACKETSIZE大小，因此在定义的时候，主机的SIZE竟可能大一点
 */

#define UpdataBase_TABLESIZE(firmMaxSize, pacSize) (firmMaxSize * 1024 / (pacSize - UpdataBase_PACKETHEADSIZE - 4) / 8 + 1)

/*Packet Struct
     __________________________________________
    |      | Type |  err  |  parmLen  |  parm  |
    |______|______|_______|___________|________|
    | byte |   1  |   1   |     2     |    N   |
    |______|______|_______|___________|________|
*/

/*File Or Flash Struct
     ________________________
    |      | info | program  |
    |______|______|__________|
    | byte |   N  |     N    |
    |______|______|__________|
*/

/* 数据包类型 */
typedef enum
{
    UpdataBase_INFO = 0,   /* 获取固件信息 */
    UpdataBase_PREPARE,    /* 准备升级 */
    UpdataBase_CODETRANS,  /* 源码传输 */
    UpdataBase_ISCOMPLETE, /* 源码完整性校验 */
    UpdataBase_JUMPAPP,    /* 跳转到应用程序 */
    UpdataBase_MAX,        /* 最大值 */
} UpdataBase_PacketType;

/* 错误列表 */
typedef enum
{
    UpdataBase_NOERR = 0,      /* 成功 */
    UpdataBase_FAIL,           /* 失败 */
    UpdataBase_TRANSFINISH,    /* 传输完成 */
    UpdataBase_INFOCRCERR,     /* 信息区域CRC校验错误 */
    UpdataBase_PTYPEERR,       /* 数据包类型错误 */
    UpdataBase_PLENERR,        /* 数据包长度错误 */
    UpdataBase_NOPREPAREERR,   /* 更新未就绪错误 */
    UpdataBase_INFOVERIFYERR,  /* 信息段校验错误 */
    UpdataBase_PROVERIFYERR,   /* 程序段校验错误 */
    UpdataBase_CODESIZEERR,    /* 程序长度错误 */
    UpdataBase_TABLEOUT,       /* 进度表超范围 */
    UpdataBase_NOHANDLE,       /* 不存在相应的处理函数 */
    UpdataBase_GETCODEDATAERR, /* 获取源码错误 */
    UpdataBase_UPDATAMARK,     /* 固件更新 */
    UpdataBase_TRANSKEYERR,    /* 传输秘钥错误 */
    UpdataBase_INFOADDRERR,    /* 信息区域地址错误 */
    UpdataBase_JUMPISNULL,     /* 程序跳转函数为空 */
    UpdataBase_FIRMADDRERR,    /* 固件路径错误 */
} UpdataBase_StatusList;

#pragma pack(1)

struct SUpdataBase_Packet
{
    uint8_t type;
    uint8_t err;
    uint16_t parmLen;
    uint8_t pParm[UpdataBase_PACKETSIZE];
};
typedef struct SUpdataBase_Packet UpdataBase_Packet; /* UpdataBase_Packet 类型 */
typedef UpdataBase_Packet *PUpdataBase_Packet;       /* PUpdataBase_Packet 指针类型 */

/* 固件信息，注意结构体大小不能超过 UpdataBase_INFOSIZE 设定的大小 */
struct SUpdataBase_FirmInfo
{
    uint16_t infoCRC;                        /* 信息区域的CRC校验 */
    uint32_t deviceAddress;                  /* 设备地址 */
    uint32_t infoAddr;                       /* 信息区域位置 */
    uint32_t infoSize;                       /* 信息区域大小 */
    uint32_t proAddr;                        /* 程序区域位置 */
    uint32_t proSize;                        /* 程序区域大小 */
    uint8_t version[8];                      /* 版本号 */
    uint16_t nameLen;                        /* 固件名字长度，长度不代表区域大小 */
    char name[50];                           /* 存放固件名字区域 */
    volatile uint16_t verifyLen;             /* 校验长度，不代表区域大小 */
    uint8_t verify[UpdataBase_PROVERIFYLEN]; /* 存放校验结果区域 */
};
typedef struct SUpdataBase_FirmInfo UpdataBase_FirmInfo; /* UpdataBase_FirmInfo 类型 */
typedef UpdataBase_FirmInfo *PUpdataBase_FirmInfo;       /* PUpdataBase_FirmInfo 指针类型 */

#pragma pack()

extern uint32_t gUpdataBase_TimeCNT; /* 时间计数 */

/*************************************************************
** Function name:       UpdataBase_TIMECNT
** Descriptions:        时基,放在周期为1ms的函数里面执行
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define UpdataBase_TIMECNT(ms) \
    gUpdataBase_TimeCNT += (ms)

/*************************************************************
** Function name:       UpdataBase_GETSTARTTIME
** Descriptions:        获取起始时间
** Input parameters:    None
** Output parameters:   None
** Returned value:      (uint32_t)起始时间
*************************************************************/
#define UpdataBase_GETSTARTTIME(void) \
    gUpdataBase_TimeCNT

/*************************************************************
** Function name:       UpdataBase_TIMEOUT
** Descriptions:        检查超时
** Input parameters:    timeOut：(uint32_t)超时时间
**                      startTime:(uint32_t)开始的时间
** Output parameters:   None
** Returned value:      false,未超时，true，超时
*************************************************************/
#define UpdataBase_TIMEOUT(timeOut, startTime) \
    ((gUpdataBase_TimeCNT - startTime) > timeOut ? true : false)

/*************************************************************
** Function name:       UpdataBase_ELAPSETIME
** Descriptions:        获取距离起始时经过的时间
** Input parameters:    startTime:(uint32_t)开始的时间
** Output parameters:   None
** Returned value:      (uint32_t)经过的时间
*************************************************************/
#define UpdataBase_ELAPSETIME(startTime) \
    (gUpdataBase_TimeCNT - startTime)

extern void UpdataBase_ProVerifyInit(void);
extern void UpdataBase_ProDataVerify(uint8_t *pData, uint16_t len);
extern void UpdataBase_ProVerifyFinal(uint8_t *pVerify);
extern void UpdataBase_InfoVerify(PUpdataBase_FirmInfo pInfo);
extern bool UpdataBase_InfoCheck(PUpdataBase_FirmInfo pInfo);
extern uint16_t UpdataBase_PacketErrEncode(PUpdataBase_Packet pPacket, uint8_t type, uint8_t err);
extern uint16_t UpdataBase_PacketEncode(PUpdataBase_Packet pPacket, uint8_t type, uint8_t *pParm, uint16_t parmLen);
uint16_t UpdataBase_GetCodeAllNum(uint32_t firmSize, uint16_t codeSize);
extern uint8_t UpdataBase_CodeTableIsWrite(uint8_t *pTable, uint16_t codeNum);
#endif /* UpdataBase_h */

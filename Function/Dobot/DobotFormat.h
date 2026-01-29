/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotFormat.h
** Latest modified date:    2019-09-19
** Latest version:          V1.1.0
** Description:             协议格式解析
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2019-09-19
** Version:                 V1.1.0
** Descriptions:            协议格式解析
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef DobotFormat_h
#define DobotFormat_h
extern "C"
{
#include "ProtocolFrame.h"
}

#define DobotFormat_SOF 0xAA                                       /* 解析格式前导符 */
#define DobotFormat_PACKET_LENGTH 255                              /* 数据包长度 */
#define DobotFormat_PAYLOAD_LENGTH (DobotFormat_PACKET_LENGTH - 4) /* payload长度 */
#define DobotFormat_PARAMS_LENGTH (DobotFormat_PAYLOAD_LENGTH - 2) /* params长度 */

#pragma pack(1)
struct DobotFormatParm
{
    uint8_t sof1;
    uint8_t sof2;
    uint8_t length; /* pdata个数 + 2 */
    uint8_t id;
    uint8_t rw : 1;
    uint8_t isQueued : 1;
    uint8_t : 2;
    uint8_t device : 2;
    uint8_t pdata[DobotFormat_PAYLOAD_LENGTH];
};
typedef struct DobotFormatParm DobotFormatPARM; /* 定义类型*/
typedef DobotFormatPARM *PDobotFormatPARM;      /* 定义指针类型*/
#pragma pack()

typedef enum
{
    DobotFormat_Updata = 255,

    ProtocolMax = 256
} DobotFormat_IDList;

typedef enum
{
    DobotFormat_NoId = 1,
} DobotFormat_ErrCodeList;

ProtocolFrame_FORMAT_EXTERN(DobotFormat) extern uint16_t DobotFormat_PacketEncoder(PDobotFormatPARM pParm, uint8_t pDataLen);

#endif /* DobotFormat_h */

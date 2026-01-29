/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotV3UpdataClass.h
** Latest modified date:    2020-05-06
** Latest version:          V1.0.0
** Description:             固件升级类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-05-06
** Version:                 V1.0.0
** Descriptions:            固件升级类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef DobotV3UpdataClass_h
#define DobotV3UpdataClass_h
#include "UpdataClass.h"
#include "DobotV3Format.h"
extern "C"
{
#include "UpdataMaster.h"
}

class DobotV3UpdataClass : public UpdataClass
{
    Q_OBJECT
public:
    explicit DobotV3UpdataClass();
    ~DobotV3UpdataClass();
    struct
    {
        uint8_t Ver = 0;
        uint8_t Nack = 0;
        uint8_t IsAck = 0;
        uint8_t RW = 0;
        uint8_t CType = 0;
        uint8_t ENC = 0;
        uint8_t SEQ = 0;
        uint32_t SEQNum = 0;
        uint8_t SRC = 0;
        uint8_t DES = 0;
        uint8_t CmdSet = 0;
        uint8_t CmdId = 0;
    } updataParm;
    void UserExec(void);
    void UserUpdataStart(char *fileAddr, uint8_t *pHead);
    void UserUpdataFinish(void);
};

extern DobotV3UpdataClass gDobotV3UpdataClass;
UpdataMaster_EXTERN(gDobotV3UpdataParm)
#endif /* DobotV3UpdataClass_h */

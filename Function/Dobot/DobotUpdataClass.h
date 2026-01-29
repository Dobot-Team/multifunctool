/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotUpdataClass.h
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
#ifndef DobotUpdataClass_h
#define DobotUpdataClass_h
#include "UpdataClass.h"
#include "DobotFormat.h"
extern "C"
{
#include "UpdataMaster.h"
}

class DobotUpdataClass : public UpdataClass
{
    Q_OBJECT
public:
    explicit DobotUpdataClass();
    ~DobotUpdataClass();
    struct
    {
        uint8_t id = 0;
        uint8_t rw = 0;
        uint8_t isQueue = 0;
        uint8_t device = 0;
    } updataParm;
    void UserExec(void);
    void UserUpdataStart(char *fileAddr, uint8_t *pHead);
    void UserUpdataFinish(void);
};

extern DobotUpdataClass gDobotUpdataClass;
UpdataMaster_EXTERN(gDobotUpdataParm)
#endif /* DobotUpdataClass_h */

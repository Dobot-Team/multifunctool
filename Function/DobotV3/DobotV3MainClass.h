/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotV3MainClass.h
** Latest modified date:    2020-05-06
** Latest version:          V1.0.0
** Description:             
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-05-06
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
#ifndef DobotV3MainClass_h
#define DobotV3MainClass_h
#include "QThread"
#include "DobotV3ProClass.h"
#include "DobotV3UpdataClass.h"

class DobotV3MainClass : public QThread
{
public:
    DobotV3MainClass();
    ~DobotV3MainClass();

private:
    /* data */
protected:
    void run();
};

#endif /* DobotV3MainClass_h */

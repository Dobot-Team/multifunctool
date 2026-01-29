/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotV3MainClass.cpp
** Latest modified date:    2020-05-06
** Latest version:          V1.0.0
** Description:             DobotV3主类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-05-06
** Version:                 V1.0.0
** Descriptions:            DobotV3主类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "DobotV3MainClass.h"

DobotV3MainClass::DobotV3MainClass(/* args */)
{
}

DobotV3MainClass::~DobotV3MainClass()
{
}

void DobotV3MainClass::run()
{
    gDobotV3ProClass.Init();
    gDobotV3UpdataClass.Init();
    while (1)
    {
        gDobotV3ProClass.Exec();
        gDobotV3UpdataClass.Exec();
        msleep(5);
    }
}

/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotMainClass.cpp
** Latest modified date:    2020-05-06
** Latest version:          V1.0.0
** Description:             Dobot主类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-05-06
** Version:                 V1.0.0
** Descriptions:            Dobot主类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "DobotMainClass.h"

DobotMainClass::DobotMainClass(/* args */)
{
}

DobotMainClass::~DobotMainClass()
{
}

void DobotMainClass::run()
{
    gDobotProClass.Init();
    gDobotUpdataClass.Init();
    while (1)
    {
        gDobotProClass.Exec();
        gDobotUpdataClass.Exec();
        msleep(5);
    }
}

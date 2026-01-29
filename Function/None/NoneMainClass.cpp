/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               NoneMainClass.cpp
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
#include "NoneMainClass.h"
#include "NoneProClass.h"

NoneMainClass::NoneMainClass()
{
}

NoneMainClass::~NoneMainClass()
{
}

void NoneMainClass::run(void)
{
    gNoneProClass.Init();
    while (1)
    {
        gNoneProClass.Exec();
        msleep(5);
    }
}

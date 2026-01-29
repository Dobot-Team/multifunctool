/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotMainClass.h
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
#ifndef DobotMainClass_h
#define DobotMainClass_h
#include "QThread"
#include "DobotProClass.h"
#include "DobotUpdataClass.h"

class DobotMainClass : public QThread
{
public:
    DobotMainClass();
    ~DobotMainClass();

private:
    /* data */
protected:
    void run();
};

#endif /* DobotMainClass_h */

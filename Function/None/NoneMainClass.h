/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               NoneMainClass.h
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
#ifndef NoneMainClass_h
#define NoneMainClass_h
#include "QThread"
#include "DobotProClass.h"
#include "DobotUpdataClass.h"

class NoneMainClass : public QThread
{
public:
    NoneMainClass();
    ~NoneMainClass();

private:
    /* data */
protected:
    void run();
};

extern NoneMainClass gNoneMainClass;

#endif /* NoneMainClass_h */

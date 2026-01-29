/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               NoneProClass.h
** Latest modified date:    2020-04-19
** Latest version:          V1.0.0
** Description:             None 协议主文件
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-19
** Version:                 V1.0.0
** Descriptions:            None 协议主文件
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef NoneProClass_h
#define NoneProClass_h
#include "ProtocolClass.h"
#include <QObject>

class NoneProClass : public ProtocolClass
{
    Q_OBJECT
public:
    explicit NoneProClass();
    ~NoneProClass();
    uint8_t cache[1024];

private:
    void UserInit(void);
    void UserExec(void);
};

extern NoneProClass gNoneProClass;

#endif /* NoneProClass_h */

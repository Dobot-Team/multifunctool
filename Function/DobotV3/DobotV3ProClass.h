#ifndef DobotV3ProClass_H
#define DobotV3ProClass_H
#include "ProtocolClass.h"

class DobotV3ProClass : public ProtocolClass
{
    Q_OBJECT
public:
    explicit DobotV3ProClass();
    ~DobotV3ProClass();

private:
    uint16_t SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen);
    void UserInit(void);
    void UserExec(void);
};

/*************************************************************
    全局函数的变量
*************************************************************/

extern DobotV3ProClass gDobotV3ProClass;

/*************************************************************
    END 全局函数的变量
*************************************************************/

#endif // DobotV3ProClass_H

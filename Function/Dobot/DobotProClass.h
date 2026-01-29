#ifndef DobotProClass_H
#define DobotProClass_H
#include "ProtocolClass.h"

class DobotProClass : public ProtocolClass
{
    Q_OBJECT
public:
    explicit DobotProClass();
    ~DobotProClass();

private:
    uint16_t SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen);
    void UserInit(void);
    void UserExec(void);
};

/*************************************************************
    全局函数的变量
*************************************************************/

extern DobotProClass gDobotProClass;

/*************************************************************
    END 全局函数的变量
*************************************************************/

#endif // DobotProClass_H

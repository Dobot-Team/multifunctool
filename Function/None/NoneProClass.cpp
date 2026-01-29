#include "NoneProClass.h"
#include "mainwindow.h"
#include "DobotFormat.h"

NoneProClass gNoneProClass;

NoneProClass::NoneProClass()
{
    ProtocolName = "None";
}

NoneProClass::~NoneProClass()
{
}

/*************************************************************
** Function name:       NoneProClass_Exec
** Descriptions:        执行函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void NoneProClass::UserInit(void)
{
    /* 默认开启 */
    OpenExec(ProtocolName);
}

/*************************************************************
** Function name:       NoneProClass_Init
** Descriptions:        初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void NoneProClass::UserExec(void)
{
    /* 读取并显示数据 */
    uint16_t dataLen = 0;
    dataLen = ComReadData(cache, 1024);
    if (dataLen != 0)
    {
        EmitReceiveData(cache, dataLen);
    }
}

#include "DobotProClass.h"
#include "mainwindow.h"
#include "DobotFormat.h"
#include "ProtocolLink.h"
#include "DobotUpdataClass.h"
extern "C"
{
#include "ProtocolFrame.h"
#include "ProtocolPacket.h"
}

/*************************************************************
    全局函数和变量
*************************************************************/
DobotProClass gDobotProClass;

ProtocolFrame_FRAME_EXPORT(DobotFrame, 1024, 1)

    /*************************************************************
** Function name:       DobotClass_GetPacket
** Descriptions:        获取数据包回调函数
**                      pPacket:数据包指针
**                      dataLen：数据包长度
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
    void DobotClass_GetPacket(uint8_t *pPacket, uint16_t dataLen)
{
    PDobotFormatPARM pDobot = (PDobotFormatPARM)pPacket;
    if (gDobotUpdataClass.updataParm.id == pDobot->id && gDobotUpdataClass.updataParm.rw == pDobot->rw && gDobotUpdataClass.updataParm.isQueue == pDobot->isQueued)
    {
        UpdataMaster_UpdataHandle(&gDobotUpdataParm, (PUpdataBase_Packet)pDobot->pdata);
        return;
    }
    gDobotProClass.EmitReceiveData(pPacket, dataLen);
}

ProtocolPacket_EXPORT(DobotPacket, DobotClass_GetPacket)
    ProtocolFrame_FUNCLIST_EXPORT(DobotFunList, &DobotPacket)

    /*************************************************************
    END 全局函数和变量
*************************************************************/

    DobotProClass::DobotProClass()
{
    ProtocolName = "Dobot";
}

DobotProClass::~DobotProClass()
{
}

/*************************************************************
** Function name:       DobotClass::Init
** Descriptions:        初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotProClass::UserInit(void)
{
    ProtocolFrame_LINK_FORMAT_FUNCLIST(DobotFrame, ProtocolLink, DobotFormat, DobotFunList);
}

/*************************************************************
** Function name:       DobotClass::Exec
** Descriptions:        执行函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotProClass::UserExec(void)
{
    ProtocolFrame_EXEC(DobotFrame);
}

/*************************************************************
** Function name:       SoltSendData_FormatTrans
** Descriptions:        格式转换
** Input parameters:    pData:需要转换的数据指针
**                      dataLen：需要转换的数据长度
** Output parameters:   None
** Returned value:      转换后需要发送的数据长度
** Remarks:             None
*************************************************************/
uint16_t DobotProClass::SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen)
{
    qDebug() << "formattrans";
    Pmainwindow_DobotSData pDobotWindow = (Pmainwindow_DobotSData)pData;
    DobotFormatPARM dobot = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    dobot.id = pDobotWindow->ID;
    dobot.rw = pDobotWindow->rw;
    dobot.isQueued = pDobotWindow->isQueue;
    dobot.device = pDobotWindow->device;
    memcpy(dobot.pdata, pDobotWindow->param, dataLen - 3);
    dataLen = DobotFormat_PacketEncoder(&dobot, dataLen - 3);
    memcpy(pData, &dobot, dataLen);
    return dataLen;
}

#include "DobotV3ProClass.h"
#include "mainwindow.h"
#include "DobotV3Format.h"
#include "ProtocolLink.h"
#include "DobotV3UpdataClass.h"
extern "C"
{
#include "ProtocolFrame.h"
#include "ProtocolPacket.h"
}

/*************************************************************
    全局函数和变量
*************************************************************/
DobotV3ProClass gDobotV3ProClass;

ProtocolFrame_FRAME_EXPORT(DobotV3Frame, 1024, 1)

    /*************************************************************
** Function name:       DobotV3Class_GetPacket
** Descriptions:        获取数据包回调函数
**                      pPacket:数据包指针
**                      dataLen：数据包长度
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
    void DobotV3Class_GetPacket(uint8_t *pPacket, uint16_t dataLen)
{
    PDobotV3Format_Packet pDobotV3 = (PDobotV3Format_Packet)pPacket;
    if (gDobotV3UpdataClass.updataParm.Ver == pDobotV3->version && gDobotV3UpdataClass.updataParm.Nack == pDobotV3->needAck && gDobotV3UpdataClass.updataParm.IsAck == pDobotV3->isAck && gDobotV3UpdataClass.updataParm.RW == pDobotV3->rw && gDobotV3UpdataClass.updataParm.CType == pDobotV3->cmdType && gDobotV3UpdataClass.updataParm.ENC == pDobotV3->encType && gDobotV3UpdataClass.updataParm.SEQ == pDobotV3->seqType && gDobotV3UpdataClass.updataParm.SEQNum == pDobotV3->seqNum && gDobotV3UpdataClass.updataParm.SRC == pDobotV3->des && gDobotV3UpdataClass.updataParm.DES == pDobotV3->src && gDobotV3UpdataClass.updataParm.CmdSet == pDobotV3->cmdSet && gDobotV3UpdataClass.updataParm.CmdId == pDobotV3->cmdId)
    {
        UpdataMaster_UpdataHandle(&gDobotV3UpdataParm, (PUpdataBase_Packet)pDobotV3->payloadAndCRC);
        return;
    }
    gDobotV3ProClass.EmitReceiveData(pPacket, dataLen);
}

ProtocolPacket_EXPORT(DobotV3Packet, DobotV3Class_GetPacket)
    ProtocolFrame_FUNCLIST_EXPORT(DobotV3FunList, &DobotV3Packet)

    /*************************************************************
    END 全局函数和变量
*************************************************************/

    DobotV3ProClass::DobotV3ProClass()
{
    ProtocolName = "DobotV3";
}

DobotV3ProClass::~DobotV3ProClass()
{
}

/*************************************************************
** Function name:       DobotV3Class::Init
** Descriptions:        初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3ProClass::UserInit(void)
{
    ProtocolFrame_LINK_FORMAT_FUNCLIST(DobotV3Frame, ProtocolLink, DobotV3Format, DobotV3FunList);
}

/*************************************************************
** Function name:       DobotV3Class::Exec
** Descriptions:        执行函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3ProClass::UserExec(void)
{
    ProtocolFrame_EXEC(DobotV3Frame);
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
uint16_t DobotV3ProClass::SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen)
{
    Pmainwindow_DobotV3SData pDobotV3Window = (Pmainwindow_DobotV3SData)pData;
    DobotV3Format_Packet dobotV3;
    memset(&dobotV3, 0, sizeof(DobotV3Format_Packet));
    dobotV3.version = pDobotV3Window->Ver;
    dobotV3.needAck = pDobotV3Window->Nack;
    dobotV3.isAck = pDobotV3Window->IsAck;
    dobotV3.rw = pDobotV3Window->RW;
    dobotV3.cmdType = pDobotV3Window->CType;
    dobotV3.encType = pDobotV3Window->ENC;
    dobotV3.seqType = pDobotV3Window->SEQ;
    dobotV3.seqNum = pDobotV3Window->SEQNum;
    dobotV3.src = pDobotV3Window->SRC;
    dobotV3.des = pDobotV3Window->DES;
    dobotV3.cmdSet = pDobotV3Window->CmdSet;
    dobotV3.cmdId = pDobotV3Window->CmdId;
    memcpy(dobotV3.payloadAndCRC, pDobotV3Window->PayLoad, dataLen - (pDobotV3Window->PayLoad - &pDobotV3Window->Ver));
    dataLen = DobotV3Format_PacketEncoder(&dobotV3, dataLen - (pDobotV3Window->PayLoad - &pDobotV3Window->Ver));
    memcpy(pData, &dobotV3, dataLen);
    return dataLen;
}

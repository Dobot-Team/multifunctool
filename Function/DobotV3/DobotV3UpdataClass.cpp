/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotV3UpdataClassClass.cpp
** Latest modified date:    2020-05-06
** Latest version:          V1.0.0
** Description:             固件升级类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-05-06
** Version:                 V1.0.0
** Descriptions:            固件升级类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "DobotV3UpdataClass.h"
#include "UpdataClass.h"
#include "DobotV3ProClass.h"
#include "DobotV3Format.h"
#include "mainwindow.h"

/*************************************************************
    全局函数和变量
*************************************************************/
DobotV3UpdataClass gDobotV3UpdataClass;

/*************************************************************
** Function name:       DobotV3UpdataClass_DevInfoCB
** Descriptions:        设备信息回调函数
** Input parameters:    pInfo：固件信息指针,为NULL表示固件信息为空
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_DevInfoCB(PUpdataBase_FirmInfo pInfo)
{

    return;
}

/*************************************************************
** Function name:       DobotV3UpdataClass_UpdataFinishCB
** Descriptions:        更新结束回调
** Input parameters:    pInfo：更新完成后的固件信息指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_UpdataFinishCB(PUpdataBase_FirmInfo pInfo)
{
    return;
}

/*************************************************************
** Function name:       DobotV3UpdataClass_UpdataErrCB
** Descriptions:        更新错误回调
** Input parameters:    updataStep：更新的步骤
**                      errType：错误类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_UpdataErrCB(uint8_t updataStep, uint8_t errType)
{
    return;
}

/*************************************************************
** Function name:       DobotV3UpdataClass_UpdataStepChangeCB
** Descriptions:        更新步骤跳转
** Input parameters:    curStep:当前步骤
**                      nextStep：下一个步骤
** Output parameters:   None
** Returned value:      跳转状态，true允许跳转，false，不允许跳转，更新取消
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_UpdataStepChangeCB(uint8_t curStep, uint8_t nextStep)
{
    return;
}

/*************************************************************
** Function name:       DobotV3UpdataClass_ScheduleChangeCB
** Descriptions:        更新进度变化
** Input parameters:    pc:当前进度百分比
**                      info：当前更新进度信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_ScheduleChangeCB(float pc, char *info)
{
    gDobotV3UpdataClass.EmitSchedule(pc, info);
}

/*************************************************************
** Function name:       DobotV3UpdataClass_FuncErrCB
** Descriptions:        函数报错
** Input parameters:    funcName：函数名称
**                      errType:错误类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_FuncErrCB(const char *funcName, uint8_t errType)
{
    return;
}

UpdataMaster_CB_EXPORT(gDobotV3UpdataClassCB,
                       DobotV3UpdataClass_DevInfoCB,
                       DobotV3UpdataClass_UpdataFinishCB,
                       DobotV3UpdataClass_UpdataErrCB,
                       DobotV3UpdataClass_ScheduleChangeCB)

    /*************************************************************
** Function name:       DobotV3UpdataClass_FileOpen
** Descriptions:        打开文件
** Input parameters:    fileAddr：打开文件的地址
** Output parameters:   None
** Returned value:      true，打开成功，false，打开失败
** Remarks:             None
*************************************************************/
    bool DobotV3UpdataClass_FileOpen(char *pFileAddr)
{
    return gDobotV3UpdataClass.FileOpen(pFileAddr);
}

/*************************************************************
** Function name:       DobotV3UpdataClass_FileClose
** Descriptions:        关闭文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass_FileClose(void)
{
    gDobotV3UpdataClass.FileClose();
}

/*************************************************************
** Function name:       DobotV3UpdataClass_FileRead
** Descriptions:        文件读取
** Input parameters:    Address：读取的地址
**                      pData:读取的数据指针
**                      dataLen：读取的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t DobotV3UpdataClass_FileRead(uint32_t Address, uint8_t *pData, uint16_t dataLen)
{
    return gDobotV3UpdataClass.ReadFile(Address, pData, dataLen);
}
/*************************************************************
** Function name:       DobotV3UpdataClass_FileGetSize
** Descriptions:        获取文件大小
** Input parameters:    None
** Output parameters:   None
** Returned value:      文件大小
** Remarks:             None
*************************************************************/
uint32_t DobotV3UpdataClass_FileGetSize(void)
{
    return gDobotV3UpdataClass.GetFileSize();
}

/* 定义固件升级文件参数 */
UpdataMaster_FILE_EXPORT(gDobotV3UpdataFile, 200,
                         DobotV3UpdataClass_FileOpen,
                         DobotV3UpdataClass_FileClose,
                         DobotV3UpdataClass_FileRead,
                         DobotV3UpdataClass_FileGetSize)

    /*************************************************************
** Function name:       DobotV3UpdataClass_SendData
** Descriptions:        发送数据
** Input parameters:    pData：需要发送的数据指针
**                      len：需要发送的数据长度
**                      timeOut：超时时间
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
    void DobotV3UpdataClass_SendData(uint8_t *pData, uint16_t len)
{
    uint16_t dataLen = 0;
    DobotV3Format_Packet dobotV3 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    dobotV3.version = gDobotV3UpdataClass.updataParm.Ver;
    dobotV3.needAck = gDobotV3UpdataClass.updataParm.Nack;
    dobotV3.isAck = gDobotV3UpdataClass.updataParm.IsAck;
    dobotV3.rw = gDobotV3UpdataClass.updataParm.RW;
    dobotV3.cmdType = gDobotV3UpdataClass.updataParm.CType;
    dobotV3.encType = gDobotV3UpdataClass.updataParm.ENC;
    dobotV3.seqType = gDobotV3UpdataClass.updataParm.SEQ;
    dobotV3.seqNum = gDobotV3UpdataClass.updataParm.SEQNum;
    dobotV3.src = gDobotV3UpdataClass.updataParm.SRC;
    dobotV3.des = gDobotV3UpdataClass.updataParm.DES;
    dobotV3.cmdSet = gDobotV3UpdataClass.updataParm.CmdSet;
    dobotV3.cmdId = gDobotV3UpdataClass.updataParm.CmdId;

    memcpy(dobotV3.payloadAndCRC, pData, len);
    dataLen = DobotV3Format_PacketEncoder(&dobotV3, len);
    gDobotV3ProClass.WriteSendQueue((uint8_t *)&dobotV3, dataLen);
}

UpdataMaster_EXPORT(gDobotV3UpdataParm, DobotV3Format_PAYLOAD_LENGTH,
                    200, 2000, 200, 200,
                    DobotV3UpdataClass_SendData, gDobotV3UpdataFile, gDobotV3UpdataClassCB)

    /*************************************************************
    END 全局函数和变量
*************************************************************/

    DobotV3UpdataClass::DobotV3UpdataClass()
{
    className = "DobotV3";
}

DobotV3UpdataClass::~DobotV3UpdataClass()
{
}

/*************************************************************
** Function name:       DobotV3UpdataClass::UserExec
** Descriptions:        用户执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass::UserExec(void)
{
    UpdataMaster_UpdataExec(&gDobotV3UpdataParm);
}

/*************************************************************
** Function name:       DobotV3UpdataClass::UserExec
** Descriptions:        用户执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass::UserUpdataStart(char *fileAddr, uint8_t *pHead)
{
    qDebug() << __FUNCTION__;
    Pmainwindow_DobotV3SData pDobotV3Head = (Pmainwindow_DobotV3SData)pHead;
    updataParm.Ver = pDobotV3Head->Ver;
    updataParm.Nack = pDobotV3Head->Nack;
    updataParm.IsAck = pDobotV3Head->IsAck;
    updataParm.RW = pDobotV3Head->RW;
    updataParm.CType = pDobotV3Head->CType;
    updataParm.ENC = pDobotV3Head->ENC;
    updataParm.SEQ = pDobotV3Head->SEQ;
    updataParm.SEQNum = pDobotV3Head->SEQNum;
    updataParm.SRC = pDobotV3Head->SRC;
    updataParm.DES = pDobotV3Head->DES;
    updataParm.CmdSet = pDobotV3Head->CmdSet;
    updataParm.CmdId = pDobotV3Head->CmdId;
    qDebug() << __FUNCTION__ << updataParm.Ver;
    qDebug() << __FUNCTION__ << updataParm.DES;
    qDebug() << __FUNCTION__ << updataParm.CmdId;
    UpdataMaster_UpdataStart(&gDobotV3UpdataParm, fileAddr);
}

/*************************************************************
** Function name:       UpdataClass::UserUpdataFinish
** Descriptions:        结束固件升级
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotV3UpdataClass::UserUpdataFinish(void)
{
    UpdataMaster_UpdataFinish(&gDobotV3UpdataParm);
}

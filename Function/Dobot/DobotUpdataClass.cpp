/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               DobotUpdataClassClass.cpp
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
#include "DobotUpdataClass.h"
#include "UpdataClass.h"
#include "DobotProClass.h"
#include "DobotFormat.h"
#include "mainwindow.h"

/*************************************************************
    全局函数和变量
*************************************************************/
DobotUpdataClass gDobotUpdataClass;

/*************************************************************
** Function name:       DobotUpdataClass_DevInfoCB
** Descriptions:        设备信息回调函数
** Input parameters:    pInfo：固件信息指针,为NULL表示固件信息为空
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_DevInfoCB(PUpdataBase_FirmInfo pInfo)
{

    return;
}

/*************************************************************
** Function name:       DobotUpdataClass_UpdataFinishCB
** Descriptions:        更新结束回调
** Input parameters:    pInfo：更新完成后的固件信息指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_UpdataFinishCB(PUpdataBase_FirmInfo pInfo)
{
    return;
}

/*************************************************************
** Function name:       DobotUpdataClass_UpdataErrCB
** Descriptions:        更新错误回调
** Input parameters:    updataStep：更新的步骤
**                      errType：错误类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_UpdataErrCB(uint8_t updataStep, uint8_t errType)
{
    return;
}

/*************************************************************
** Function name:       DobotUpdataClass_UpdataStepChangeCB
** Descriptions:        更新步骤跳转
** Input parameters:    curStep:当前步骤
**                      nextStep：下一个步骤
** Output parameters:   None
** Returned value:      跳转状态，true允许跳转，false，不允许跳转，更新取消
** Remarks:             None
*************************************************************/
void DobotUpdataClass_UpdataStepChangeCB(uint8_t curStep, uint8_t nextStep)
{
    return;
}

/*************************************************************
** Function name:       DobotUpdataClass_ScheduleChangeCB
** Descriptions:        更新进度变化
** Input parameters:    pc:当前进度百分比
**                      info：当前更新进度信息
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_ScheduleChangeCB(float pc, char *info)
{
    gDobotUpdataClass.EmitSchedule(pc, info);
}

/*************************************************************
** Function name:       DobotUpdataClass_FuncErrCB
** Descriptions:        函数报错
** Input parameters:    funcName：函数名称
**                      errType:错误类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_FuncErrCB(const char *funcName, uint8_t errType)
{
    return;
}

UpdataMaster_CB_EXPORT(gDobotUpdataClassCB,
                       DobotUpdataClass_DevInfoCB,
                       DobotUpdataClass_UpdataFinishCB,
                       DobotUpdataClass_UpdataErrCB,
                       DobotUpdataClass_ScheduleChangeCB)

    /*************************************************************
** Function name:       DobotUpdataClass_FileOpen
** Descriptions:        打开文件
** Input parameters:    fileAddr：打开文件的地址
** Output parameters:   None
** Returned value:      true，打开成功，false，打开失败
** Remarks:             None
*************************************************************/
    bool DobotUpdataClass_FileOpen(char *pFileAddr)
{
    return gDobotUpdataClass.FileOpen(pFileAddr);
}

/*************************************************************
** Function name:       DobotUpdataClass_FileClose
** Descriptions:        关闭文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass_FileClose(void)
{
    gDobotUpdataClass.FileClose();
}

/*************************************************************
** Function name:       DobotUpdataClass_FileRead
** Descriptions:        文件读取
** Input parameters:    Address：读取的地址
**                      pData:读取的数据指针
**                      dataLen：读取的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t DobotUpdataClass_FileRead(uint32_t Address, uint8_t *pData, uint16_t dataLen)
{
    return gDobotUpdataClass.ReadFile(Address, pData, dataLen);
}
/*************************************************************
** Function name:       DobotUpdataClass_FileGetSize
** Descriptions:        获取文件大小
** Input parameters:    None
** Output parameters:   None
** Returned value:      文件大小
** Remarks:             None
*************************************************************/
uint32_t DobotUpdataClass_FileGetSize(void)
{
    return gDobotUpdataClass.GetFileSize();
}

/* 定义固件升级文件参数 */
UpdataMaster_FILE_EXPORT(gDobotUpdataFile, 200,
                         DobotUpdataClass_FileOpen,
                         DobotUpdataClass_FileClose,
                         DobotUpdataClass_FileRead,
                         DobotUpdataClass_FileGetSize)

    /*************************************************************
** Function name:       DobotUpdataClass_SendData
** Descriptions:        发送数据
** Input parameters:    pData：需要发送的数据指针
**                      len：需要发送的数据长度
**                      timeOut：超时时间
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
    void DobotUpdataClass_SendData(uint8_t *pData, uint16_t len)
{
    uint16_t dataLen = 0;
    DobotFormatPARM dobot = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    dobot.id = gDobotUpdataClass.updataParm.id;
    dobot.rw = gDobotUpdataClass.updataParm.rw;
    dobot.isQueued = gDobotUpdataClass.updataParm.isQueue;
    dobot.device = gDobotUpdataClass.updataParm.device;
    memcpy(dobot.pdata, pData, len);
    dataLen = DobotFormat_PacketEncoder(&dobot, len);
    gDobotProClass.WriteSendQueue((uint8_t *)&dobot, dataLen);
}

UpdataMaster_EXPORT(gDobotUpdataParm, DobotFormat_PARAMS_LENGTH,
                    200, 2000, 200, 200,
                    DobotUpdataClass_SendData, gDobotUpdataFile, gDobotUpdataClassCB)

    /*************************************************************
    END 全局函数和变量
*************************************************************/

    DobotUpdataClass::DobotUpdataClass()
{
    className = "Dobot";
}

DobotUpdataClass::~DobotUpdataClass()
{
}

/*************************************************************
** Function name:       DobotUpdataClass::UserExec
** Descriptions:        用户执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass::UserExec(void)
{
    UpdataMaster_UpdataExec(&gDobotUpdataParm);
}

/*************************************************************
** Function name:       DobotUpdataClass::UserExec
** Descriptions:        用户执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass::UserUpdataStart(char *fileAddr, uint8_t *pHead)
{
    Pmainwindow_DobotSData pDobotHead = (Pmainwindow_DobotSData)pHead;
    updataParm.id = pDobotHead->ID;
    updataParm.rw = pDobotHead->rw;
    updataParm.isQueue = pDobotHead->isQueue;
    UpdataMaster_UpdataStart(&gDobotUpdataParm, fileAddr);
}

/*************************************************************
** Function name:       UpdataClass::UserUpdataFinish
** Descriptions:        结束固件升级
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DobotUpdataClass::UserUpdataFinish(void)
{
    UpdataMaster_UpdataFinish(&gDobotUpdataParm);
}

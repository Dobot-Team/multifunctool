/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataMaster.c
** Latest modified date:    2020-04-25
** Latest version:          V1.0.0
** Description:             固件升级主机
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-25
** Version:                 V1.0.0
** Descriptions:            固件升级主机
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "UpdataMaster.h"
#include "string.h"
#include "stdio.h"

/* 存储对外输出的打印信息 */
char gUpdataMaster_InfoCache[512];

/* 更新进度百分比 */
#define UpdataMaster_START_PC UpdataBase_PREPARE /* 开始计算进度的包 */
uint8_t gUpdataMaster_UpdataPC[UpdataBase_MAX] = {
    0,
    10,
    70,
    10,
    10,
};

/*************************************************************
    Schedule
*************************************************************/

/*************************************************************
** Function name:       UpdataMaster_SetPCOne
** Descriptions:        设置一个类型的百分比
** Input parameters:    p:参数结构体指针
**                      type：类型
**                      info：信息字符串
**                      pc：百分比
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_SetPCOne(PUpdataMasterParm p, uint8_t type, char *info, uint8_t pc)
{
    p->schedule.pcOne[type] = pc;
    uint16_t allPc = 0;
    uint16_t cir = 0;
    for (cir = UpdataMaster_START_PC; cir < UpdataBase_MAX; cir++)
    {
        allPc += gUpdataMaster_UpdataPC[cir];
    }
    p->schedule.pcAll = 0;
    for (cir = UpdataMaster_START_PC; cir < type; cir++)
    {
        p->schedule.pcAll += 100 * (float)gUpdataMaster_UpdataPC[cir];
    }
    p->schedule.pcAll += (float)p->schedule.pcOne[type] * (float)gUpdataMaster_UpdataPC[type];
    p->schedule.pcAll = p->schedule.pcAll / allPc;
    if (p->CB->ScheduleChangeCB != NULL)
    {
        p->CB->ScheduleChangeCB(p->schedule.pcAll, info);
    }
}

/*************************************************************
** Function name:       UpdataMaster_SetPreparePC
** Descriptions:        设置准备升级的百分比
** Input parameters:    p:参数结构体指针
**                      status：
**                          0：正在发送升级准备指令到设备
**                          1：准备就绪
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_SetPreparePC(PUpdataMasterParm p, uint8_t status)
{
    uint8_t pc = 0;
    if (status == 0)
    {
        sprintf(gUpdataMaster_InfoCache, "Prepare To Updata");
        pc = 50;
    }
    else if (status == 1)
    {
        sprintf(gUpdataMaster_InfoCache, "Updata Ready!!!");
        pc = 100;
    }
    else
    {
        return;
    }
    UpdataMaster_SetPCOne(p, UpdataBase_PREPARE, gUpdataMaster_InfoCache, pc);
}

/*************************************************************
** Function name:       UpdataMaster_SetCodePC
** Descriptions:        设置源码传输的百分比
** Input parameters:    p:参数结构体指针
**                      curCode：当前源码包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_SetCodePC(PUpdataMasterParm p, uint16_t curCode)
{
    uint8_t pc = p->execParm.curCodeNum * 100 / (p->execParm.codeAllNum - 1);
    sprintf(gUpdataMaster_InfoCache, "CodeTotal:%d, curCode:%d", p->execParm.codeAllNum, p->execParm.curCodeNum);
    UpdataMaster_SetPCOne(p, UpdataBase_CODETRANS, gUpdataMaster_InfoCache, pc);
}

/*************************************************************
** Function name:       UpdataMaster_SetCompleteCodePC
** Descriptions:        判断源码包是否完整
** Input parameters:    p:参数结构体指针
** Output parameters:   status：
**                          0：发送固件完整性指令
**                          1：固件已接收完成
**                          2：固件接收未完整
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_SetCompleteCodePC(PUpdataMasterParm p, uint8_t status)
{
    uint8_t pc = 0;
    if (status == 0)
    {
        sprintf(gUpdataMaster_InfoCache, "Check The Code Is Complete");
        pc = 50;
    }
    else if (status == 1)
    {
        sprintf(gUpdataMaster_InfoCache, "Code Is Complete!!!");
        pc = 100;
    }
    else if (status == 2)
    {
        sprintf(gUpdataMaster_InfoCache, "Code Is Incomplete,Resend The Missing Firmware Code!!!");
        pc = 0;
    }
    else
    {
        return;
    }
    UpdataMaster_SetPCOne(p, UpdataBase_ISCOMPLETE, gUpdataMaster_InfoCache, pc);
}

/*************************************************************
** Function name:       UpdataMaster_SetCompleteCodePC
** Descriptions:        判断源码包是否完整
** Input parameters:    p:参数结构体指针
** Output parameters:   status：
**                          0：发送跳转指令
**                          1：跳转成功
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_SetJumpAppPC(PUpdataMasterParm p, uint8_t status)
{
    uint8_t pc = 0;
    if (status == 0)
    {
        sprintf(gUpdataMaster_InfoCache, "Try To Jump To App");
        pc = 50;
    }
    else if (status == 1)
    {
        sprintf(gUpdataMaster_InfoCache, "App Is Runing!!!");
        pc = 100;
    }
    else
    {
        return;
    }
    UpdataMaster_SetPCOne(p, UpdataBase_JUMPAPP, gUpdataMaster_InfoCache, pc);
}

/*************************************************************
    END Schedule
*************************************************************/

/*************************************************************
** Function name:       UpdataMaster_UpdataStart
** Descriptions:        开始升级
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_UpdataStart(PUpdataMasterParm p, char *pFileAddr)
{
    /* 清空所有执行参数 */
    memset(&p->execParm, 0, sizeof(UpdataMasterExecParm));
    /* 清空所有进度参数 */
    memset(&p->schedule, 0, sizeof(UpdataMasterSchedule));
    p->schedule.updataStep = UpdataBase_PREPARE; /* 设置更新步骤 */
    p->schedule.updataMark = true;               /* 设置更新标记 */
    strncpy(p->file->pFileAddr, pFileAddr, p->file->fileAddrMax);
}

/*************************************************************
** Function name:       UpdataMaster_UpdataFinish
** Descriptions:        更新结束
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_UpdataFinish(PUpdataMasterParm p)
{
    p->schedule.updataMark = false;
}

/*************************************************************
** Function name:       UpdataMaster_PacketClear
** Descriptions:        数据包状态清空
** Input parameters:    p:参数结构体指针
**                      type：数据包类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_PacketClear(PUpdataMasterParm p, uint8_t type)
{
    p->execParm.cmdSendMark[type] = false;
    p->execParm.sendTimeStart[type] = 0;
    p->execParm.ackMark[type] = false;
}

/*************************************************************
** Function name:       UpdataMaster_StepChange
** Descriptions:        步骤改变
** Input parameters:    p:参数结构体指针
**                      curStep：当前步骤
**                      nextStep：下一个步骤
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_STEPCHANGECB(p, curStep, nextStep) \
    UpdataMaster_PacketClear(p, curStep);               \
    UpdataMaster_PacketClear(p, nextStep);              \
    p->schedule.updataStep = nextStep

/*************************************************************
** Function name:       UpdataMaster_UPDATAERRCB
** Descriptions:        更新错误回调函数,一旦出错，则停止更新
** Input parameters:    p：(PUpdataMasterParm)参数指针
**                      updataStep：更新步骤
**                      errType：错误类型
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_UPDATAERRCB(p, updataStep, errType)                                   \
    UpdataMaster_UpdataFinish(p);                                                          \
    sprintf(gUpdataMaster_InfoCache, "Updata Err, step:%d, type:%d", updataStep, errType); \
    if (p->CB->ScheduleChangeCB != NULL)                                                   \
    {                                                                                      \
        p->CB->ScheduleChangeCB(0, gUpdataMaster_InfoCache);                               \
    }                                                                                      \
    if (p->CB->UpdataErrCB != NULL)                                                        \
    {                                                                                      \
        p->CB->UpdataErrCB(updataStep, errType);                                           \
    };                                                                                     \
    return

/*************************************************************
** Function name:       UpdataMaster_FILEOPEN
** Descriptions:        打开文件
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataMaster_FILEOPEN(p)                    \
    if (p->file->Open(p->file->pFileAddr) == false) \
    {                                               \
        return UpdataBase_FIRMADDRERR;              \
    }

/*************************************************************
** Function name:       UpdataMaster_SendPacket
** Descriptions:        数据发送
** Input parameters:    p:参数结构体指针
**                      type：类型
**                      pParm：参数指针
**                      parmLen：参数长度
** Output parameters:   None
** Returned value:      具体查看 UpdataMaster_SendStatus
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_SendPacket(PUpdataMasterParm p, uint8_t type, uint8_t *pParm, uint16_t parmLen)
{
    if (p->execParm.cmdSendMark[type] == true)
    {
        if (UpdataBase_TIMEOUT(p->pCmdTime[type], p->execParm.sendTimeStart[type]) == false)
        {
            return UpdataMaster_SENDING;
        }
        UpdataMaster_PacketClear(p, type);
        return UpdataMaster_NOACK;
    }
    if (p->execParm.ackMark[type] == true)
    {
        UpdataMaster_PacketClear(p, type);
        return UpdataMaster_SUCCESS;
    }
    UpdataMaster_PacketClear(p, type);
    uint16_t len = UpdataBase_PacketEncode((PUpdataBase_Packet)p->pPacketCache, type, pParm, parmLen);
    p->pLink->SendData(p->pPacketCache, len);
    p->execParm.sendTimeStart[type] = UpdataBase_GETSTARTTIME();
    p->execParm.cmdSendMark[type] = true;
    return UpdataMaster_SENDING;
}

/*************************************************************
** Function name:       UpdataMaster_PacketIsSend
** Descriptions:        判断数据包是否发送
** Input parameters:    p:参数结构体指针
**                      type：类型
** Output parameters:   None
** Returned value:      false：未发送，true 发送
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_PacketIsSend(PUpdataMasterParm p, uint8_t type)
{
    return p->execParm.cmdSendMark[type];
}

/*************************************************************
** Function name:       UpdataMaster_ReadInfo
** Descriptions:        读取信息
** Input parameters:    p:参数结构体指针
**                      pInfo：固件信息指针
** Output parameters:   None
** Returned value:      true:读取成功，false读取失败
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_ReadInfo(PUpdataMasterParm p, PUpdataBase_FirmInfo pInfo)
{
    UpdataMaster_FILEOPEN(p);
    p->file->Read(0, (uint8_t *)pInfo, sizeof(UpdataBase_FirmInfo));
    return UpdataBase_InfoCheck(pInfo);
    p->file->Close();
}

/*************************************************************
** Function name:       UpdataMaster_GetProVerify
** Descriptions:        程序段校验
** Input parameters:    p:参数结构体指针
**                      pInfo:需要校验的固件信息参数
**                      pVerify：存放校验结果的指针
** Output parameters:   None
** Returned value:      错误信息 UpdataBase_StatusList
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_GetProVerify(PUpdataMasterParm p, PUpdataBase_FirmInfo pInfo, uint8_t *pVerify)
{
    uint32_t finishAddr = pInfo->infoSize + pInfo->proSize;
    uint32_t curAddr = pInfo->infoSize;
    uint32_t readSize = 0;
    UpdataBase_ProVerifyInit();
    UpdataMaster_FILEOPEN(p);
    while (curAddr < finishAddr)
    {
        if ((finishAddr - curAddr) > UpdataMaster_VERIFYCACHE)
        {
            readSize = UpdataMaster_VERIFYCACHE;
        }
        else
        {
            readSize = finishAddr - curAddr;
        }
        p->file->Read(curAddr, p->file->verifyCache, readSize);
        curAddr += readSize;
        UpdataBase_ProDataVerify(p->file->verifyCache, readSize);
    }
    p->file->Close();
    UpdataBase_ProVerifyFinal(pVerify);
    return UpdataBase_NOERR;
}

/*************************************************************
** Function name:       UpdataMaster_FirmVerify
** Descriptions:        固件校验
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_FirmVerify(PUpdataMasterParm p)
{
    uint8_t verify[UpdataBase_PROVERIFYLEN]; /* 存放校验结果 */
    uint8_t returnValue = 0;
    /* 读取固件信息 */
    if (UpdataMaster_ReadInfo(p, &p->infoCache) == false)
    {
        return UpdataBase_INFOVERIFYERR;
    }
    /* 校验固件大小与信息区域存储的大小是否一致 */
    uint32_t firmSize = p->infoCache.infoSize + p->infoCache.proSize;
    if (p->file->GetSize() != firmSize)
    {
        return UpdataBase_INFOVERIFYERR;
    }
    printf("verifylen: %d", p->infoCache.verifyLen);
    /* 检验固件信息的校验长度与预设的长度是否一致 */
    if (p->infoCache.verifyLen != UpdataBase_PROVERIFYLEN)
    {
        return UpdataBase_PROVERIFYERR;
    }
    /* 校验固件程序段的校验信息是否正确 */
    returnValue = UpdataMaster_GetProVerify(p, &p->infoCache, verify);
    if (returnValue != UpdataBase_NOERR)
    {
        return returnValue;
    }
    if (memcmp(&p->infoCache.verify, &verify, UpdataBase_PROVERIFYLEN) != 0)
    {
        return UpdataBase_PROVERIFYERR;
    }
    return UpdataBase_NOERR;
}

/*************************************************************
** Function name:       UpdataMaster_GetCodeParm
** Descriptions:        获取需要传输的源码数据包内容
** Input parameters:    p:参数结构体指针
**                      pData：数据指针
**                      pLen:数据长度指针
** Output parameters:   None
** Returned value:      UpdataBase_NOERR ：获取成功
**                      UpdataBase_FAIL ：获取失败
**                      UpdataBase_TRANSFINISH ：更新完成
**                      UpdataBase_FIRMADDRERR ：固件路径错误
** Remarks:             None
*************************************************************/
uint8_t UpdataMaster_GetCodeParm(PUpdataMasterParm p, uint8_t *pData, uint16_t *pLen)
{
    uint16_t cir = p->execParm.curCodeNum;
    uint16_t codeLen = 0;
    memcpy(pData, &p->execParm.transKey, 2);
    for (cir = p->execParm.curCodeNum; cir < p->execParm.codeAllNum; cir++)
    {
        if (UpdataBase_CodeTableIsWrite(p->execParm.updataTable, cir) == false)
        {
            p->execParm.curCodeNum = cir;
            memcpy(pData + 2, &p->execParm.curCodeNum, 2);
            UpdataMaster_FILEOPEN(p);
            codeLen = p->file->Read(p->execParm.transCodeSize * cir, pData + 4, p->execParm.transCodeSize);
            p->file->Close();
            *pLen = codeLen + 4;
            if (codeLen == p->execParm.transCodeSize)
            {
                return UpdataBase_NOERR;
            }
            if (codeLen == 0 || cir != (p->execParm.codeAllNum - 1))
            {
                return UpdataBase_FAIL;
            }
            return UpdataBase_NOERR;
        }
    }
    p->execParm.curCodeNum = cir;
    return UpdataBase_TRANSFINISH;
}

/*************************************************************
** Function name:       UpdataMaster_InfoHandle
** Descriptions:        信息包处理函数
** Input parameters:    p:参数结构体指针
**                      pRead:接收到的数据包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_InfoHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead)
{
    if (p->CB->DevInfoCB != NULL)
    {
        if (pRead->parmLen == 0)
        {
            p->CB->DevInfoCB(NULL);
        }
        else if (pRead->parmLen == sizeof(UpdataBase_FirmInfo))
        {
            p->CB->DevInfoCB((PUpdataBase_FirmInfo)pRead->pParm);
        }
        else
        {
            UpdataMaster_UPDATAERRCB(p, UpdataBase_INFO, UpdataBase_PLENERR);
        }
    }
}

/*************************************************************
** Function name:       UpdataMaster_PrepareHandle
** Descriptions:        准备升级处理函数
** Input parameters:    p:参数结构体指针
**                      pRead:接收到的数据包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_PrepareHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead)
{
    if (pRead->parmLen != 8)
    {
        UpdataMaster_UPDATAERRCB(p, UpdataBase_PREPARE, UpdataBase_PLENERR);
    }
    /* 获取传输秘钥 */
    memcpy(&p->execParm.transKey, (uint8_t *)pRead->pParm, 2);
    /* 设置传输间隔 */
    p->pCmdTime[UpdataBase_CODETRANS] = *(uint16_t *)(pRead->pParm + 2);
    /* 设置传输源码大小 */
    memcpy(&p->execParm.transCodeSize, ((uint8_t *)pRead->pParm) + 4, 2);
    if (p->execParm.transCodeSize > UpdataBase_CODEPACSIZE)
    {
        UpdataMaster_UPDATAERRCB(p, UpdataBase_PREPARE, UpdataBase_CODESIZEERR);
    }
    /* 获取传输的数据包总数 */
    memcpy(&p->execParm.codeAllNum, ((uint8_t *)pRead->pParm) + 6, 2);
    UpdataMaster_STEPCHANGECB(p, p->schedule.updataStep, p->schedule.updataStep + 1);
    UpdataMaster_SetPreparePC(p, 1);
}

/*************************************************************
** Function name:       源码完整性指令处理
** Descriptions:        UpdataMaster_CodeCompleteHandle
** Input parameters:    p:参数结构体指针
**                      pRead:接收到的数据包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_CodeCompleteHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead)
{
    if (pRead->parmLen == 0)
    { /* 传输完成 */
        UpdataMaster_STEPCHANGECB(p, UpdataBase_ISCOMPLETE, UpdataBase_JUMPAPP);
        UpdataMaster_SetCompleteCodePC(p, 1);
        return;
    }
    if (pRead->parmLen > UpdataBase_PACKETPARMSIZE)
    {
        UpdataMaster_UPDATAERRCB(p, UpdataBase_ISCOMPLETE, UpdataBase_TABLEOUT);
    }
    memcpy(p->execParm.updataTable, pRead->pParm, pRead->parmLen);
    UpdataMaster_STEPCHANGECB(p, UpdataBase_ISCOMPLETE, UpdataBase_CODETRANS);
    p->execParm.curCodeNum = 0;
    UpdataMaster_SetCompleteCodePC(p, 2);
}

/*************************************************************
** Function name:       UpdataBoot_JumpAppHandle
** Descriptions:        跳转到应用程序
** Input parameters:    p:参数结构体指针
**                      pRead:接收到的数据包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataBoot_JumpAppHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead)
{
    if (pRead->parmLen != sizeof(UpdataBase_FirmInfo))
    {
        UpdataMaster_UPDATAERRCB(p, UpdataBase_JUMPAPP, UpdataBase_PLENERR);
    }
    if (p->CB->UpdataFinishCB != NULL)
    {
        p->CB->UpdataFinishCB((PUpdataBase_FirmInfo)pRead->pParm);
    }
    UpdataMaster_SetJumpAppPC(p, 1);
    UpdataMaster_UpdataFinish(p);
}

/*************************************************************
** Function name:       UpdataMaster_GetDevInfo
** Descriptions:        获取设备信息
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      获取状态:
**                          UpdataMaster_SUCCESS 已经正确获取到
**                          UpdataMaster_SENDING 正在获取
**                          UpdataMaster_NOACK   设备无应答
** Remarks:             获取的信息通过 DevInfoCB 回调函数得到
*************************************************************/
uint8_t UpdataMaster_GetDevInfo(PUpdataMasterParm p)
{
    return UpdataMaster_SendPacket(p, UpdataBase_INFO, NULL, 0);
}

/*************************************************************
** Function name:       UpdataMaster_PrepareExec
** Descriptions:        更新准备执行程序
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_PrepareExec(PUpdataMasterParm p)
{
    static uint8_t firstMark = false;
    uint8_t err = UpdataBase_NOERR;
    if (p->schedule.updataStep != UpdataBase_PREPARE)
    {
        firstMark = false;
        return;
    }
    if (firstMark == false)
    {
        err = UpdataMaster_FirmVerify(p); /* 这里会读取info存入infoCache，下面可以直接使用infoCache */
        if (err != UpdataBase_NOERR)
        {
            UpdataMaster_UPDATAERRCB(p, UpdataBase_PREPARE, err);
        }
    }
    if (UpdataMaster_PacketIsSend(p, UpdataBase_PREPARE) == false)
    {
        UpdataMaster_SetPreparePC(p, 0);
    }
    firstMark = true;
    UpdataMaster_SendPacket(p, UpdataBase_PREPARE, (uint8_t *)&p->infoCache, sizeof(UpdataBase_FirmInfo));
}

/*************************************************************
** Function name:       UpdataMaster_CodeExec
** Descriptions:        源码传输执行程序
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_CodeExec(PUpdataMasterParm p)
{
    uint16_t len = 0;
    uint8_t result = 0;
    if (p->schedule.updataStep != UpdataBase_CODETRANS)
    {
        return;
    }
    if (UpdataMaster_PacketIsSend(p, UpdataBase_CODETRANS) == false)
    {
        result = UpdataMaster_GetCodeParm(p, ((PUpdataBase_Packet)p->pPacketCache)->pParm, &len);
        if (result == UpdataBase_TRANSFINISH)
        {
            UpdataMaster_STEPCHANGECB(p, p->schedule.updataStep, p->schedule.updataStep + 1);
            return;
        }
        else if (result == UpdataBase_NOERR)
        {
            UpdataMaster_SetCodePC(p, p->execParm.curCodeNum);
        }
        else
        {
            UpdataMaster_UPDATAERRCB(p, p->schedule.updataStep, result);
        }
    }
    UpdataMaster_SendPacket(p, UpdataBase_CODETRANS, ((PUpdataBase_Packet)p->pPacketCache)->pParm, len);
}

/*************************************************************
** Function name:       UpdataMaster_CodeCompleteExec
** Descriptions:        源码完整性检查执行程序
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_CodeCompleteExec(PUpdataMasterParm p)
{
    if (p->schedule.updataStep != UpdataBase_ISCOMPLETE)
    {
        return;
    }
    if (UpdataMaster_PacketIsSend(p, UpdataBase_ISCOMPLETE) == false)
    {
        UpdataMaster_SetCompleteCodePC(p, 0);
    }
    UpdataMaster_SendPacket(p, UpdataBase_ISCOMPLETE, (uint8_t *)&p->execParm.transKey, 2);
}

/*************************************************************
** Function name:       UpdataMaster_JumpAppExec
** Descriptions:        跳转到App的执行程序
** Input parameters:    p:参数结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_JumpAppExec(PUpdataMasterParm p)
{
    if (p->schedule.updataStep != UpdataBase_JUMPAPP)
    {
        return;
    }
    if (UpdataMaster_PacketIsSend(p, UpdataBase_JUMPAPP) == false)
    {
        UpdataMaster_SetJumpAppPC(p, 0);
    }
    UpdataMaster_SendPacket(p, UpdataBase_JUMPAPP, NULL, 0);
}

const UpdataMasterHandle gUpdataMaster_Handle[] = {
    UpdataBase_INFO,
    UpdataMaster_InfoHandle,
    UpdataBase_PREPARE,
    UpdataMaster_PrepareHandle,
    UpdataBase_ISCOMPLETE,
    UpdataMaster_CodeCompleteHandle,
    UpdataBase_JUMPAPP,
    UpdataBoot_JumpAppHandle,
};
const uint8_t gUpdataMaster_HandleNum = sizeof(gUpdataMaster_Handle) / sizeof(UpdataMasterHandle);

/*************************************************************
** Function name:       UpdataMaster_UpdataHandle
** Descriptions:        升级指令处理
** Input parameters:    p:参数结构体指针
**                      pRead:接收到的数据包
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void UpdataMaster_UpdataHandle(PUpdataMasterParm p, PUpdataBase_Packet pRead)
{
    if (pRead->type >= UpdataBase_MAX)
    {
        return;
    }
    p->execParm.ackMark[pRead->type] = true;
    if (pRead->err != UpdataBase_NOERR)
    {
        UpdataMaster_UPDATAERRCB(p, p->schedule.updataStep, pRead->err);
    }
    for (uint8_t cir = 0; cir < gUpdataMaster_HandleNum; cir++)
    {
        if (gUpdataMaster_Handle[cir].type == pRead->type)
        {
            gUpdataMaster_Handle[cir].Handle(p, pRead);
            return;
        }
    }
    UpdataMaster_UPDATAERRCB(p, p->schedule.updataStep, UpdataBase_NOHANDLE);
}

/*************************************************************
** Function name:       UpdataMaster_UpdataExec
** Descriptions:        升级执行函数
** Input parameters:    pCmd：指令指针
**                      需要发送的数据指针
** Output parameters:   None
** Returned value:      需要发送的数据长度
** Remarks:             None
*************************************************************/
void UpdataMaster_UpdataExec(PUpdataMasterParm p)
{
    /* 判断是否需要更新 */
    if (p->schedule.updataMark == false)
    {
        return;
    }
    UpdataMaster_PrepareExec(p);
    UpdataMaster_CodeExec(p);
    UpdataMaster_CodeCompleteExec(p);
    UpdataMaster_JumpAppExec(p);
}

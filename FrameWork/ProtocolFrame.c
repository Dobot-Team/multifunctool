/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolFrame.c
** Latest modified date:    2019-12-16
** Latest version:          V2.0.0
** Description:             自定义协议框架
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2019-09-19
** Version:                 V1.0.0
** Descriptions:            自定义协议框架
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "ProtocolFrame.h"
#include <string.h>

uint32_t gProtocolFrame_TimeCNT; /* 时间计数 */

/*************************************************************
** Function name:       ProtocolFrame_MSGCLEAR
** Descriptions:        消息清除
** Input parameters:    pLink：(PProtocolFrame_LinkPARM)链路指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_MSGCLEAR(pLink) \
    pLink->msgReadIndex = 0;          \
    pLink->msgWriteIndex = 0

/*************************************************************
** Function name:       ProtocolFrame_MSGWRITE
** Descriptions:        解析链路消息缓存写入
** Input parameters:    pLink：(PProtocolFrame_LinkPARM)链路指针
**                      msg：(uint8_t)写入消息
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_MSGWRITE(pLink, msg)         \
    pLink->pDeMsgBuff[pLink->msgWriteIndex] = msg; \
    pLink->msgWriteIndex++;

/*************************************************************
** Function name:       ProtocolFrame_MSGREAD
** Descriptions:        解析链路消息读取
** Input parameters:    pLink：(PProtocolFrame_LinkPARM)链路指针
**                      msg：(uint8_t)读取消息
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_MSGREAD(pLink, msg)         \
    msg = pLink->pDeMsgBuff[pLink->msgReadIndex]; \
    pLink->msgReadIndex++;

/*************************************************************
** Function name:       ProtocolFrame_MSGSETREADINDEX
** Descriptions:        设置读取索引指针
** Input parameters:    pLink：解析链路指针
**                      execIndex：读取索引
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_MSGSETREADINDEX(pLink, execIndex) \
    pLink->msgReadIndex = execIndex;

/*************************************************************
** Function name:       ProtocolFrame_STEPCLEAR
** Descriptions:        清除解析链路的解析步骤
** Input parameters:    pLink：(PProtocolFrame_LinkPARM)链路指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define ProtocolFrame_STEPCLEAR(pLink) \
    pLink->deStep = 0;                 \
    pLink->deLenght = 0;               \
    pLink->sofMark = 0

/*************************************************************
** Function name:       ProtocolFrame_LinkMsgToZero
** Descriptions:        将消息移动到0地址位置
** Input parameters:    pLink：解析链路指针
**                      moveIndex：移动的索引起始地址
**                      lenght：移动的长度
** Output parameters:   None
** Returned value:      None
*************************************************************/
void ProtocolFrame_LinkMsgToZero(
    PProtocolFrame_LinkPARM pLink,
    uint32_t moveIndex,
    uint32_t lenght)
{
    uint32_t cir;
    for (cir = 0; cir < lenght; cir++)
    {
        pLink->pDeMsgBuff[cir] = pLink->pDeMsgBuff[moveIndex];
        moveIndex++;
    }
    pLink->msgWriteIndex = lenght;
}

/*************************************************************
** Function name:       ProtocolFrame_PacketParmFill
** Descriptions:        填充数据包参数
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink:解析链路
**                      pPacket:数据包指针
**                      packetLength：数据包长度
** Output parameters:   no
** Returned             no
*************************************************************/
void ProtocolFrame_PacketParmFill(PProtocolFramePARM pFrame,
                                  PProtocolFrame_LinkPARM pLink,
                                  uint8_t *pPacket,
                                  uint16_t packetLength)
{
    PProtocolFrame_PacketPARM pPacketParm = pFrame->pPacketParm;
    pPacketParm->pPacket = pFrame->pPacket;
    if (pPacket != NULL)
    {
        memcpy(pPacketParm->pPacket, pPacket, packetLength);
    }
    pPacketParm->packetLength = packetLength;
    pPacketParm->sendMark = false;
    pPacketParm->pLink = pLink;
    pPacketParm->exitCallPack = false;
    pPacketParm->errCode = 0;
}

/*************************************************************
** Function name:       ProtocolFrame_FuncMarkClear
** Descriptions:        功能标记清除
** Input parameters:    pFrame：框架参数结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void ProtocolFrame_FuncMarkClear(PProtocolFramePARM pFrame)
{
    uint8_t cir = 0;
    PProtocolFrame_FuncLIST pFuncList = NULL; /* 功能列表 */
    for (cir = 0; cir < pFrame->linkNum; cir++)
    {
        pFuncList = pFrame->pLinkList[cir]->pFuncList;
        memset(pFuncList->pFuncParmList, 0, sizeof(ProtocolFrame_FuncPARM) * pFuncList->funcListSize);
    }
}

/*************************************************************
** Function name:       ProtocolFrame_FuncLoop
** Descriptions:        功能循环执行
** Input parameters:    pFrame：框架参数结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void ProtocolFrame_FuncLoop(PProtocolFramePARM pFrame)
{
    PProtocolFrame_FuncLIST pFuncList = NULL;
    PProtocolFrame_PacketPARM pPacketParm = pFrame->pPacketParm;
    uint8_t listSize = 0;
    uint8_t cir1 = 0;
    uint8_t cir2 = 0;
    ProtocolFrame_FuncMarkClear(pFrame);
    for (cir1 = 0; cir1 < pFrame->linkNum; cir1++)
    {
        pFuncList = pFrame->pLinkList[cir1]->pFuncList;
        listSize = pFuncList->funcListSize;
        for (cir2 = 0; cir2 < listSize; cir2++)
        {
            if (pFuncList->pFuncParmList[cir2].loopMark == true)
            {
                continue;
            }
            if (pFuncList->pFuncStructList[cir2]->Loop == NULL)
            {
                pFuncList->pFuncParmList[cir2].loopMark = true;
                continue;
            }
            while (1)
            {
                ProtocolFrame_PacketParmFill(pFrame, NULL, NULL, 0);
                pFuncList->pFuncStructList[cir2]->Loop(pFuncList->pFuncStructList[cir2], pPacketParm);
                /* 判断发送链路，发送标记 */
                if (pPacketParm->pLink != NULL && pPacketParm->sendMark == true)
                {
                    /* 判断是否需要发送错误代码 */
                    if (pPacketParm->errCode != 0)
                    {
                        if (pPacketParm->pLink->pFormat->ErrCodeSend != NULL)
                        {
                            pPacketParm->pLink->pFormat->ErrCodeSend(pPacketParm);
                        }
                    }
                    pPacketParm->pLink->SendData(pPacketParm->pPacket, pPacketParm->packetLength);
                }
                if (pPacketParm->exitCallPack == true)
                {
                    break;
                }
            }
            pFuncList->pFuncParmList[cir2].loopMark = true;
        }
    }
}

/*************************************************************
** Function name:       ProtocolFrame_FuncCallBack
** Descriptions:        数据包处理
** Input parameters:    pPacketParm：数据包参数指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void ProtocolFrame_FuncCallBack(PProtocolFrame_PacketPARM pPacketParm)
{
    PProtocolFrame_FuncLIST pFuncList = pPacketParm->pLink->pFuncList;
    uint8_t listSize = pFuncList->funcListSize;
    uint8_t cir = 0;
    for (cir = 0; cir < listSize; cir++)
    {
        if (pFuncList->pFuncStructList[cir]->PacketCallBack == NULL)
        {
            continue;
        }
        pFuncList->pFuncStructList[cir]->PacketCallBack(pFuncList->pFuncStructList[cir], pPacketParm);
        if (pPacketParm->sendMark == true)
        {
            if (pPacketParm->errCode != 0)
            {
                /* 发送错误代码 */
                if (pPacketParm->pLink->pFormat->ErrCodeSend != NULL)
                {
                    pPacketParm->pLink->pFormat->ErrCodeSend(pPacketParm);
                }
            }
            pPacketParm->pLink->SendData(pPacketParm->pPacket, pPacketParm->packetLength);
        }
        if (pPacketParm->exitCallPack == true)
        {
            return;
        }
    }
}

/*************************************************************
** Function name:       ProtocolFrame_DecodeInMsgBuf
** Descriptions:        从消息buf解析数据
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink：数据链路指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void ProtocolFrame_DecodeInMsgBuf(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    uint8_t data = 0;
    uint32_t msgStartIndex = 0;
    uint8_t stepCheckStatus = 0; /* 步骤校验的状态 */
    PProtocolFrame_FormatPARM pDeFormat = pLink->pFormat;
    ProtocolFrame_STEPCLEAR(pLink); /* 清空解析步骤 */
    ProtocolFrame_MSGSETREADINDEX(pLink, 1);
    uint8_t *pMsgBuf;
    while (pLink->msgReadIndex < pLink->msgWriteIndex)
    {
        ProtocolFrame_MSGREAD(pLink, data);
        /* 判断是否接收到前导符 */
        if (pLink->sofMark == false)
        {
            if (data == pDeFormat->sof)
            {
                msgStartIndex = pLink->msgReadIndex - 1;
                pLink->deLenght++;
                pLink->sofMark = true; /* 置位sofMark标记 */
                pMsgBuf = pLink->pDeMsgBuff + msgStartIndex;
                pLink->deStepLength = pDeFormat->pStepList[pLink->deStep].CheckLength(pMsgBuf, pLink->deLenght);
                pLink->deStartTime = ProtocolFrame_GETSTARTTIME();
            }
            continue;
        }
        pLink->deLenght++;
        /* 判断是否接收到解析长度的数据 */
        if (pLink->deLenght < pLink->deStepLength)
        {
            continue;
        }
        stepCheckStatus = pDeFormat->pStepList[pLink->deStep].StepCheck(pMsgBuf, pLink->deLenght);
        /* 判断解析是否错误 */
        if (stepCheckStatus == ProtocolFrame_DEFALSE)
        {
            ProtocolFrame_MSGSETREADINDEX(pLink, msgStartIndex + 1);
            ProtocolFrame_STEPCLEAR(pLink);
            continue;
        }
        pLink->deStep++;
        /* 判断是否解析完所有步骤 */
        if (pLink->deStep < pDeFormat->stepListSize)
        {
            pLink->deStepLength = pDeFormat->pStepList[pLink->deStep].CheckLength(pMsgBuf, pLink->deLenght);
            continue;
        }
        ProtocolFrame_PacketParmFill(pFrame, pLink, pMsgBuf, pLink->deLenght);
        if (pDeFormat->ErrCodeRead != NULL)
        {
            pDeFormat->ErrCodeRead(pFrame->pPacketParm);
        }
        ProtocolFrame_FuncCallBack(pFrame->pPacketParm);
        ProtocolFrame_STEPCLEAR(pLink); /* 清空解析步骤 */
    }
    ProtocolFrame_LinkMsgToZero(pLink, msgStartIndex, pLink->deLenght);
}

/*************************************************************
** Function name:       ProtocolFrame_DecodeInLink
** Descriptions:        从数据链路解析数据
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink：数据链路指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void ProtocolFrame_DecodeInLink(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    uint8_t data = 0;
    uint8_t stepCheckStatus = 0; /* 步骤校验的状态 */
    /* 检查链路是否使能 */
    if (pLink->enable == false)
    {
        while (pLink->readData(&data, 1) == 1)
            ;
        ProtocolFrame_STEPCLEAR(pLink); /* 清空解析步骤 */
        ProtocolFrame_MSGCLEAR(pLink);  /* 清空消息内容 */
        return;
    }
    PProtocolFrame_FormatPARM pDeFormat = pLink->pFormat;
    while (pLink->readData(&data, 1) == 1)
    {
        /* 判断是否接收到前导符 */
        if (pLink->sofMark == false)
        {
            if (data == pDeFormat->sof)
            {
                ProtocolFrame_MSGWRITE(pLink, data);
                pLink->deLenght++;
                pLink->sofMark = true;
                pLink->deStepLength = pDeFormat->pStepList[pLink->deStep].CheckLength(pLink->pDeMsgBuff, pLink->deLenght);
                pLink->deStartTime = ProtocolFrame_GETSTARTTIME();
            }
            continue;
        }
        pLink->deLenght++;
        ProtocolFrame_MSGWRITE(pLink, data); /* 将数据写入消息buf */
        ;
        /* 判断是否接收到解析长度的数据 */
        if (pLink->deLenght < pLink->deStepLength)
        {
            continue;
        }
        /* 步骤校验 */
        stepCheckStatus = pDeFormat->pStepList[pLink->deStep].StepCheck(pLink->pDeMsgBuff, pLink->deLenght);
        /* 判断解析是否错误 */
        if (stepCheckStatus == ProtocolFrame_DEFALSE)
        {
            ProtocolFrame_DecodeInMsgBuf(pFrame, pLink);
            continue;
        }
        pLink->deStep++;
        /* 判断是否解析完所有步骤 */
        if (pLink->deStep < pDeFormat->stepListSize)
        {
            pLink->deStepLength = pDeFormat->pStepList[pLink->deStep].CheckLength(pLink->pDeMsgBuff, pLink->deLenght);
            continue;
        }
        ProtocolFrame_PacketParmFill(pFrame, pLink, pLink->pDeMsgBuff, pLink->deLenght);
        if (pDeFormat->ErrCodeRead != NULL)
        {
            pDeFormat->ErrCodeRead(pFrame->pPacketParm);
        }
        ProtocolFrame_FuncCallBack(pFrame->pPacketParm);
        ProtocolFrame_STEPCLEAR(pLink); /* 清空解析步骤 */
        ProtocolFrame_MSGCLEAR(pLink);  /* 清空消息内容 */
    }
    if (ProtocolFrame_TIMEOUT(pLink->deTimeOut, pLink->deStartTime) == true)
    { /* 判断解析是否超时，超时则解析下一帧数据 */
        ProtocolFrame_DecodeInMsgBuf(pFrame, pLink);
    }
}

/*************************************************************
** Function name:       ProtocolFrame_DeLinkDecode
** Descriptions:        链路解析
** Input parameters:    pFrame：框架参数结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void ProtocolFrame_DeLinkDecode(PProtocolFramePARM pFrame)
{
    uint8_t cir = 0;
    for (cir = 0; cir < pFrame->linkNum; cir++)
    {
        ProtocolFrame_DecodeInLink(pFrame, pFrame->pLinkList[cir]);
    }
}

/*************************************************************
** Function name:       ProtocolFrame_SearchLinkInList
** Descriptions:        检查链路是否在注册表位置
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink:查找的链路参数结构体指针
** Output parameters:   no
** min：                no
** Returned             false,链路未注册，true链路已经注册
*************************************************************/
uint8_t ProtocolFrame_SearchLinkInList(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    uint16_t cirNum = pFrame->linkNum;
    while (cirNum)
    {
        cirNum--;
        if (pFrame->pLinkList[cirNum] == pLink)
        {
            return true;
        }
    }
    return false;
}

/*************************************************************
** Function name:       ProtocolFrame_RegisterLink
** Descriptions:        注册链路
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink:需要注册的链路指针
** Output parameters:   no
** min：                no
** Returned             true，成功
**                      false，失败
*************************************************************/
uint8_t ProtocolFrame_RegisterLink(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    if (ProtocolFrame_SearchLinkInList(pFrame, pLink) == true)
    {
        return true;
    }
    if (pFrame->linkNum >= pFrame->linkMaxNum || pLink == NULL || pLink->readData == NULL || pLink->SendData == NULL || pLink->pDeMsgBuff == NULL)
    {
        return false;
    }
    else
    {
        ProtocolFrame_STEPCLEAR(pLink);
        ProtocolFrame_MSGCLEAR(pLink);
        pFrame->pLinkList[pFrame->linkNum] = pLink;
        pFrame->linkNum++;
        return true;
    }
}

/*************************************************************
** Function name:       ProtocolFrame_Link_Format_FuncList
** Descriptions:        链接框架，链路，格式，功能列表
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink:链路指针
**                      pFormat:解析格式指针
**                      pFuncList：功能列表指针
** Output parameters:   no
** min：                no
** Returned             true，成功
**                      false，失败
*************************************************************/
void ProtocolFrame_Link_Format_FuncList(PProtocolFramePARM pFrame,
                                        PProtocolFrame_LinkPARM pLink,
                                        PProtocolFrame_FormatPARM pFormat,
                                        PProtocolFrame_FuncLIST pFuncList)
{
    if (ProtocolFrame_RegisterLink(pFrame, pLink) == true)
    {
        pLink->pFormat = pFormat;
        pLink->pFuncList = pFuncList;
    }
}

/*************************************************************
** Function name:       ProtocolFrame_FrameLinkEnableOnly
** Descriptions:        仅使能 协议框架 指定链路
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink：不禁能的链路指针
** Output parameters:   no
** min：                no
** Returned             true，成功
**                      false，失败
*************************************************************/
void ProtocolFrame_FrameLinkEnableOnly(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    uint8_t cir = 0;
    for (cir = 0; cir < pFrame->linkNum; cir++)
    {
        if (pFrame->pLinkList[cir] != pLink)
        {
            pFrame->pLinkList[cir]->enable = false;
        }
        else
        {
            pFrame->pLinkList[cir]->enable = true;
        }
    }
}

/*************************************************************
** Function name:       ProtocolFrame_FrameLinkDisableOnly
** Descriptions:        仅禁能 协议框架 指定链路
** Input parameters:    pFrame：框架参数结构体指针
**                      pLink：不禁能的链路指针
** Output parameters:   no
** min：                no
** Returned             true，成功
**                      false，失败
*************************************************************/
void ProtocolFrame_FrameLinkDisableOnly(PProtocolFramePARM pFrame, PProtocolFrame_LinkPARM pLink)
{
    uint8_t cir = 0;
    for (cir = 0; cir < pFrame->linkNum; cir++)
    {
        if (pFrame->pLinkList[cir] != pLink)
        {
            pFrame->pLinkList[cir]->enable = true;
        }
        else
        {
            pFrame->pLinkList[cir]->enable = false;
        }
    }
}

/*************************************************************
** Function name:       ProtocolFrame_exec
** Descriptions:        协议包框架的执行程序
** Input parameters:    pFrame：框架参数结构体指针
** Output parameters:   no
** min：                no
** Returned             no
*************************************************************/
void ProtocolFrame_exec(PProtocolFramePARM pFrame)
{
    ProtocolFrame_DeLinkDecode(pFrame);
    ProtocolFrame_FuncLoop(pFrame);
}

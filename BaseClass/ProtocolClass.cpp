/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               ProtocolClass.cpp
** Latest modified date:    2020-04-19
** Latest version:          V1.0.0
** Description:             协议主线程
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-19
** Version:                 V1.0.0
** Descriptions:            协议主线程
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "ProtocolClass.h"

ProtocolClass::ProtocolClass()
{
    dataBufLen = 1024;
    pDataBuf = new uint8_t[dataBufLen];
    sendQueue.clear();
}

ProtocolClass::~ProtocolClass()
{
    delete pDataBuf;
}

/*************************************************************
** Function name:       ProtocolClass::UserInit
** Descriptions:        用户初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::UserInit(void)
{
    return;
}

/*************************************************************
** Function name:       ProtocolClass::Init
** Descriptions:        初始化函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::Init(void)
{
    UserInit();
    return;
}

/*************************************************************
** Function name:       Exec
** Descriptions:        后台执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::UserExec(void)
{
    return;
}

/*************************************************************
** Function name:       Exec
** Descriptions:        后台执行程序
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::Exec(void)
{
    if (OpenExecMark == false)
    {
        return;
    }
    UserExec();
    SendQueueData();
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
uint16_t ProtocolClass::SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen)
{
    return dataLen;
}

/*************************************************************
** Function name:       ProtocolClass::SoltSendData
** Descriptions:        发送数据信号处理
** Input parameters:    pData：字节数据
**                      dataLen:字节数据长度
**                      string：字符数据
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::SoltSendData(uint8_t *pData, uint16_t dataLen)
{
    if (OpenExecMark == false)
    {
        return;
    }
    uint8_t *pCache = new uint8_t[ProtocolClass_PACMAXNUM];
    memcpy(pCache, pData, dataLen);
    dataLen = SoltSendData_FormatTrans(pCache, dataLen);
    WriteSendQueue(pCache, dataLen);
    delete pCache;
}

/*************************************************************
** Function name:       ProtocolClass::EmitReceiveData
** Descriptions:        发送接收到数据的信号
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::EmitReceiveData(uint8_t *pData, uint16_t dataLen)
{
    emit SignalReceiveData(pData, dataLen);
}

/*************************************************************
** Function name:       ProtocolClass::ComReadData
** Descriptions:        串口读取数据
** Input parameters:    pData：需要读取的数据
**                      dataLen：需要读取的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t ProtocolClass::ComReadData(uint8_t *pData, uint16_t dataLen)
{
    uint16_t readLen = 0;
    emit ComReadDataSignal(pData, dataLen, &readLen);
    return readLen;
}

/*************************************************************
** Function name:       ProtocolClass::ComSendData
** Descriptions:        串口发送数据
** Input parameters:    pData：需要发送的数据
**                      dataLen：需要发送的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::ComSendData(uint8_t *pData, uint16_t dataLen)
{
    emit ComSendDataSignal(pData, dataLen);
}

/*************************************************************
** Function name:       ProtocolClass::enQueue
** Descriptions:        数据入队
** Input parameters:    queue：队列参数
**                      pData：需要入队的数据
**                      dataLen：需要入队的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::enQueue(QQueue<uint8_t> &queue, uint8_t *pData, uint16_t dataLen)
{
    uint16_t index = 0;
    while (index < dataLen)
    {
        queue.enqueue(pData[index]);
        index++;
    }
}

/*************************************************************
** Function name:       ProtocolClass::enQueue
** Descriptions:        数据入队
** Input parameters:    queue：队列参数
**                      pData：需要入队的数据
**                      dataLen：需要入队的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t ProtocolClass::deQueue(QQueue<uint8_t> &queue, uint8_t *pData, uint16_t dataLen)
{
    uint16_t index = 0;
    while ((!queue.isEmpty()) && (index < dataLen))
    {
        pData[index] = queue.dequeue();
        index++;
    }
    return index;
}

/*************************************************************
** Function name:       ProtocolClass::WriteSendQueue
** Descriptions:        将数据写入发送队列
** Input parameters:    pData：需要发送的数据指针
**                      dataLen：需要发送的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::WriteSendQueue(uint8_t *pData, uint16_t dataLen)
{
    enQueue(sendQueue, pData, dataLen);
}

/*************************************************************
** Function name:       DobotClass::SendQueueData
** Descriptions:        发送队列的数据
** Input parameters:    pData：需要发送的数据指针
**                      dataLen：需要发送的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::SendQueueData(void)
{
    uint16_t dataLen = 0;
    dataLen = deQueue(sendQueue, pDataBuf, dataBufLen);
    if (dataLen != 0)
    {
        emit ComSendData(pDataBuf, dataLen);
    }
}

/*************************************************************
** Function name:       ProtocolClass::OpenExec
** Descriptions:        开启执行
** Input parameters:    isOpen：true 开启，fasle 不开启
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ProtocolClass::OpenExec(QString Protocol)
{
    if (Protocol == ProtocolName)
    {
        OpenExecMark = true;
    }
    else
    {
        OpenExecMark = false;
    }
}

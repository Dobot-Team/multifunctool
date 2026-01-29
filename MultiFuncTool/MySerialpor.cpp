/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               MySerialPor.cpp
** Latest modified date:    2020-04-16
** Latest version:          V1.0.0
** Description:             串口读写函数
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-16
** Version:                 V1.0.0
** Descriptions:            串口读写函数
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "MySerialPor.h"

MySerialPor::MySerialPor()
{
    pPort = new QSerialPort;
}


MySerialPor::~MySerialPor()
{
    delete pPort;
}

/*************************************************************
** Function name:       MySerialPor::run
** Descriptions:        线程运行
** Input parameters:    portName：端口名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::run()
{
    while(1)
    {
        SerialErr_BGP();
        ComRefresh_BGP();
        msleep(5);
    }
}

/*************************************************************
** Function name:       MySerialPor::SerialOpen
** Descriptions:        串口开启函数
** Input parameters:    portName：端口名称
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::Open(QString portName,
                       qint32 baudRate,
                       QSerialPort::DataBits dataBits,
                       QSerialPort::Parity parity,
                       QSerialPort::StopBits stopBits)
{
    if(portName == NULL){
        emit ErrSignal("请选择 端口");
        return;
    }
    if (pPort->isOpen()){
        return;
    }
    //设置端口名
    pPort->setPortName(portName);
    //设置波特率
    pPort->setBaudRate(baudRate);
    //设置数据位数
    pPort->setDataBits(dataBits);
    //设置奇偶校验
    pPort->setParity(parity);
    //设置停止位
    pPort->setStopBits(stopBits);
    //设置流控制
    pPort->setFlowControl(QSerialPort::SoftwareControl);
    pPort->setRequestToSend(true);
    pPort->setDataTerminalReady(true);
    //打开串口
    pPort->open(QIODevice::ReadWrite);
    qDebug() << "serial is open " << pPort->isOpen();
    if (pPort->isOpen()){
        /* 发送串口开启的信号 */
        emit StatusSignal(true);
    }

}

void MySerialPor::Open(QString portName)
{
    if(portName == NULL){
        emit ErrSignal("请选择 端口");
        return;
    }
    if (pPort->isOpen()){
        return;
    }
    //设置端口名
    pPort->setPortName(portName);
    //设置波特率
    pPort->setBaudRate(QSerialPort::Baud115200);
    //设置数据位数
    pPort->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    pPort->setParity(QSerialPort::NoParity);
    //设置停止位
    pPort->setStopBits(QSerialPort::OneStop);
    //设置流控制
    pPort->setFlowControl(QSerialPort::SoftwareControl);
    pPort->setRequestToSend(true);
    pPort->setDataTerminalReady(true);
    //打开串口
    pPort->open(QIODevice::ReadWrite);
    qDebug() << "serial is open " << pPort->isOpen();
    if (pPort->isOpen()){
        /* 发送串口开启的信号 */
        emit StatusSignal(true);
    }

}
/*************************************************************
** Function name:       MySerialPor::SerialClose
** Descriptions:        串口关闭函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::Close(void)
{
    if(pPort != NULL){
        if(pPort->isOpen()){
            pPort->close();
        }
        emit StatusSignal(false);
    }
}

/*************************************************************
** Function name:       MySerialPor::SerialErr
** Descriptions:        串口错误函数，后台循环执行
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::SerialErr_BGP(void)
{
    uint8_t error = pPort->error();
    if(error == QSerialPort::NoError){
        return;
    }else{
        switch(error){
            case QSerialPort::DeviceNotFoundError:
                emit ErrSignal("未找到相应的设备");
                break;
            case QSerialPort::PermissionError:
                emit ErrSignal("串口被占用");
                break;
            case QSerialPort::WriteError:
            case QSerialPort::ReadError:
                emit ErrSignal("串口操作失败");
                break;
            case QSerialPort::ResourceError:
                emit ErrSignal("串口异常并终止");
                Close();
                break;
        }
        pPort->clearError();
    }
}

/*************************************************************
** Function name:       MySerialPor::ComRefresh_BGP
** Descriptions:        端口刷新
** Input parameters:    None
** Output parameters:   None
** Returned value:      true 端口刷新，false 端口未刷新
** Remarks:             None
*************************************************************/
void MySerialPor::ComRefresh_BGP(void){
    QStringList curList;
    /* 获取当前串口列表 */
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        curList << info.portName();
    }
    /* 对比新旧两个串口列表 */
    bool find = false;
    bool update = false;
    if (ComList.length() != curList.length()){
        update = true;
    }else{
        foreach (const QString name_old, ComList) {
            foreach (const QString name_new, curList) {
                if (name_old == name_new){
                    find = true;
                    break;
                }
            }
            if (find == false){
                update = true;
            }
        }
    }
    /* 检查列表是否更新 */
    if (update){
        ComList = curList;
        emit ComRefresh(ComList);
    }
}


/*************************************************************
** Function name:       MySerialPor::ReadData
** Descriptions:        串口读取函数，后台循环执行，读取的数据放入环形队列
** Input parameters:    pBuf:需要接收的数据指针
**                      len：需要接收的数据长度
**                      pReadLen:读取的数据长度指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::ReadData(uint8_t *pBuf,uint16_t len,uint16_t *pReadLen)
{
    if(pPort->isOpen() && pPort->size() != 0){
        QByteArray readBuff;
        readBuff = pPort->read(len);
        while(*pReadLen != readBuff.length()){
            pBuf[*pReadLen] = readBuff.constData()[*pReadLen];
            (*pReadLen)++;
        }
    }
}

/*************************************************************
** Function name:       MySerialPor::SendData
** Descriptions:        串口数据发送函数
** Input parameters:    pBuf:需要发送的数据指针
**                      len：需要发送的数据长度
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void MySerialPor::SendData(uint8_t *pBuf,uint16_t len)
{
    if(pPort->isOpen()){
        pPort->write((char *)pBuf,len);
    }
}


















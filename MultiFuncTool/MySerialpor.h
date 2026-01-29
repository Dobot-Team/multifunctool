/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               MySerialPor.h
** Latest modified date:    2020-04-16
** Latest version:          V1.0.0
** Description:             串口读写功能
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-16
** Version:                 V1.0.0
** Descriptions:            串口读写功能
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef MySerialPor_h
#define MySerialPor_h

#include <QObject>
#include <QtSerialPort>
#include <QTime>
#include "stdint.h"
#include <QQueue>

class MySerialPor : public QThread
{
    Q_OBJECT
public:
    explicit MySerialPor();
    ~MySerialPor();
    void Open(QString portname);
signals:
    void StatusSignal(bool status); /*true 打开，false 关闭*/
    void ErrSignal(QString err);
    void ComRefresh(QStringList list);
public slots:
    void Open(QString portName,
               qint32 baudRate,
               QSerialPort::DataBits dataBits,
               QSerialPort::Parity parity,
               QSerialPort::StopBits stopBits);
    void Close(void);
    void ReadData(uint8_t *pBuf,uint16_t len,uint16_t *pReadLen);
    void SendData(uint8_t *pBuf,uint16_t len);
private:
    QSerialPort *pPort;
    QStringList ComList;
    void SerialErr_BGP(void);
    void ComRefresh_BGP(void);
protected:
    void run();
};

#endif /* MySerialPor_h */


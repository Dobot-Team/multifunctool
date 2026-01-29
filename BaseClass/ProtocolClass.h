#ifndef ProtocolClass_H
#define ProtocolClass_H
#include <QtCore>
#include <QThread>
#include <QObject>
#include <QQueue>

/* 数据包最大长度 */
#define ProtocolClass_PACMAXNUM 1024

/*************************************************************
** Function name:       ProtocolClass_CONNECT
** Descriptions:        连接界面，放在主函数里面定义
** Input parameters:    className:须符合变量命名规范
**                      ProtocolClass：协议类
**                      cWindow：界面类，界面类里面需包含SendSignal(QString,uint8_t,uint8_t*,QString)信号
**                      WSendSignal：界面类发送信号
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define ProtocolClass_CONNECT(protocol, mainWindow, serial)                                 \
    QObject::connect(&mainWindow, SIGNAL(ProtocolOpenExec(QString)),                        \
                     &protocol, SLOT(OpenExec(QString)));                                   \
    QObject::connect(&mainWindow, SIGNAL(ProtocolSendData(uint8_t *, uint16_t)),            \
                     &protocol, SLOT(SoltSendData(uint8_t *, uint16_t)));                   \
    QObject::connect(&protocol, SIGNAL(SignalReceiveData(uint8_t *, uint16_t)),             \
                     &mainWindow, SLOT(DataShow(uint8_t *, uint16_t)),                      \
                     Qt::BlockingQueuedConnection);                                         \
    QObject::connect(&protocol, SIGNAL(ComReadDataSignal(uint8_t *, uint16_t, uint16_t *)), \
                     &serial, SLOT(ReadData(uint8_t *, uint16_t, uint16_t *)),              \
                     Qt::BlockingQueuedConnection);                                         \
    QObject::connect(&protocol, SIGNAL(ComSendDataSignal(uint8_t *, uint16_t)),             \
                     &serial, SLOT(SendData(uint8_t *, uint16_t)),                          \
                     Qt::BlockingQueuedConnection);

class ProtocolClass : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolClass();
    ~ProtocolClass();
    QString ProtocolName;
    bool OpenExecMark = false;

    uint16_t ComReadData(uint8_t *pData, uint16_t dataLen);
    void WriteSendQueue(uint8_t *pData, uint16_t dataLen);

    void EmitReceiveData(uint8_t *pData, uint16_t dataLen);
    void Init(void);
    void Exec(void);
    virtual void UserExec(void);
    virtual void UserInit(void);
    virtual uint16_t SoltSendData_FormatTrans(uint8_t *pData, uint16_t dataLen);
signals:
    void ComReadDataSignal(uint8_t *pData, uint16_t dataLen, uint16_t *pReadLen);
    void ComSendDataSignal(uint8_t *pData, uint16_t dataLen);
    void SignalReceiveData(uint8_t *pData, uint16_t dataLen);
public slots:
    void OpenExec(QString Protocol);

private:
    uint8_t *pDataBuf;
    uint16_t dataBufLen;
    QQueue<uint8_t> sendQueue; /* 数据发送队列 */
    void SendQueueData(void);
    void enQueue(QQueue<uint8_t> &queue, uint8_t *pData, uint16_t dataLen);
    void ComSendData(uint8_t *pData, uint16_t dataLen);
    void SendWindowSignal(QString protocol, uint8_t type, uint8_t *pData, uint16_t dataLen);
    uint16_t deQueue(QQueue<uint8_t> &queue, uint8_t *pData, uint16_t dataLen);
private slots:
    void SoltSendData(uint8_t *pData, uint16_t dataLen);
};

#endif // ProtocolClass_H

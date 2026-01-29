/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               UpdataClass.h
** Latest modified date:    2020-04-29
** Latest version:          V1.0.0
** Description:             固件更新类
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2020-04-29
** Version:                 V1.0.0
** Descriptions:            固件更新类
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef UpdataClass_h
#define UpdataClass_h
#include <QtCore>
#include <QFile>
#include <QTime>
extern "C"
{
#include "UpdataMaster.h"
#include "md5.h"
}

/*************************************************************
** Function name:       UpdataClass_CONNECT
** Descriptions:        连接界面，放在主函数里面定义
** Input parameters:    Updata：固件升级类
**                      mainWindow：界面类
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define UpdataClass_CONNECT(Updata, mainWindow)                                     \
    QObject::connect(&mainWindow, SIGNAL(ProtocolOpenExec(QString)),                \
                     &Updata, SLOT(OpenExec(QString)));                             \
    QObject::connect(&mainWindow, SIGNAL(StartFirmUpdata(bool, char *, uint8_t *)), \
                     &Updata, SLOT(UpdataStartSolt(bool, char *, uint8_t *)));      \
    QObject::connect(&Updata, SIGNAL(UpdataInfoShow(QString)),                      \
                     &mainWindow, SLOT(InfoShow(QString)),                          \
                     Qt::BlockingQueuedConnection);                                 \
    QObject::connect(&Updata, SIGNAL(UpdataSchedule(float)),                        \
                     &mainWindow, SLOT(FirmUpdataSchedule(float)),                  \
                     Qt::BlockingQueuedConnection);                                 \
    QObject::connect(&Updata, SIGNAL(UpdataFinishSignal(float)),                    \
                     &mainWindow, SLOT(FirmUpdataSchedule(float)),                  \
                     Qt::BlockingQueuedConnection);

class UpdataClass : public QObject
{
    Q_OBJECT
public:
    UpdataClass(void);
    ~UpdataClass();
    bool OpenExecMark = false;
    QString className;
    void EmitUpdataErr(QString errInfo);
    void EmitSchedule(float pc, char *info);
    void EmitUpdataFinish(void);
    bool FirmMake(QString binAddr, QString dfirmAddr,
                  char *pName,
                  uint32_t deviceAddr,
                  uint8_t *pVerison,
                  uint32_t infoAddr,
                  uint32_t proAddr);
    bool FileOpen(char *pFileAddr);
    void FileClose(void);
    uint16_t ReadFile(uint32_t Address, uint8_t *pData, uint16_t dataLen);
    uint32_t GetFileSize(void);
    void Init(void);
    void Exec(void);
    virtual void UserInit(void);
    virtual void UserExec(void);
    virtual void UserUpdataStart(char *fileAddr, uint8_t *pHead);
    virtual void UserUpdataFinish(void);
public slots:
    void UpdataStartSolt(bool isStart, char *pFirmFileAddr, uint8_t *pHead);
    void OpenExec(QString Name);
signals:
    void UpdataInfoShow(QString errInfo);
    void UpdataSchedule(float pc);
    void UpdataFinishSignal(void);

private:
    /* data */
    QFile firmFile;
};

#endif /* UpdataClass_h */

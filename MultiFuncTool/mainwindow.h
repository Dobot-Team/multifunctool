#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>

#define mainwindow_DOBOTSDATA_PARAMLEN 512
#define mainwindow_DOBOTV3SDATA_PARAMLEN 512

struct Smainwindow_DobotSData
{
    uint8_t ID;
    uint8_t rw;
    uint8_t isQueue;
    uint8_t device;
    uint8_t param[mainwindow_DOBOTSDATA_PARAMLEN];
};
typedef struct Smainwindow_DobotSData mainwindow_DobotSData; /* mainwindow_DobotSData 类型 */
typedef mainwindow_DobotSData *Pmainwindow_DobotSData;       /* Pmainwindow_DobotSData 指针类型 */

struct Smainwindow_DobotV3SData
{
    uint8_t Ver;
    uint8_t Nack;
    uint8_t IsAck;
    uint8_t RW;
    uint8_t CType;
    uint8_t ENC;
    uint8_t SEQ;
    uint32_t SEQNum;
    uint8_t SRC;
    uint8_t DES;
    uint8_t CmdSet;
    uint8_t CmdId;
    uint8_t PayLoad[mainwindow_DOBOTV3SDATA_PARAMLEN];
};
typedef struct Smainwindow_DobotV3SData mainwindow_DobotV3SData; /* mainwindow_DobotV3SData 类型 */
typedef mainwindow_DobotV3SData *Pmainwindow_DobotV3SData;       /* Pmainwindow_DobotV3SData 指针类型 */

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void BurnV3Firmware(QString fileAddr, QString version);
    ~MainWindow();

signals:
    void ComOpen(QString portName,
                 qint32 baudRate,
                 QSerialPort::DataBits dataBits,
                 QSerialPort::Parity parity,
                 QSerialPort::StopBits stopBits);
    void ComClose(void);
    /* 协议相关的信号 */
    void ProtocolOpenExec(QString protocol);
    void ProtocolSendData(uint8_t *pData, uint16_t len);
    void FirmwareShow(QString file);
    void FirmwareShow(QString file, QString version);
    void StartFirmUpdata(bool isStart, char *pFileAddr, uint8_t *pHead);
public slots:
    void ComStatus(bool status);
    void ComRefresh(QStringList list);
    void ShowFirmAddr(QString fileAddr);
    void DataShow(uint8_t *pData, uint16_t dataLen);
    void InfoShow(QString info);
    void FirmUpdataSchedule(float pc);
    void FirmUdataFinish(void);
    void startUpdate_slot();
private slots:
    void BackGroundProgram(void);
    void TimeCNT(void);
    void on_ProtocolType_currentIndexChanged(const QString &arg1);
    void ProtocolTypeChanged();

    void on_ComOpen_clicked();

    void on_Dobot_FirmFile_clicked();

    void on_pushButton_5_clicked();

    void on_NoneClear_clicked();

    void on_NoneSend_clicked();

    void on_DataShow_cursorPositionChanged();

    void on_ClearDataShow_clicked();

    void on_Dobot_FirmUpdata_clicked();

    void on_DobotV3_Send_clicked();

    void on_DobotV3_FirmUpdata_clicked();

    void on_DobotV3_FirmFile_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *pBGPTimer;
    QTimer *pTimeCNT;
    bool ProtocolTypeAddItemMark = false;
    QStringList ComList;
    struct SQWidgetTabName
    {
        QWidget *tab;
        QString name;
    };
    char fileAddr[300];
    mainwindow_DobotSData dobotSendData;
    mainwindow_DobotV3SData dobotV3SendData;
    typedef struct SQWidgetTabName QWidgetTabName; /* QWidgetTabName 类型 */
    typedef QWidgetTabName *PQWidgetTabName;       /* Pmainwindow_QWidgetTable 指针类型 */
    PQWidgetTabName pDataTransTab = NULL;

    void DataTransShowOneTab(const QString &pTab);
    void ProtocolTypeAddItem(void);
    bool IsComOpen(void);
    uint8_t OneAscallToHex(uint8_t ascall, uint8_t *pHex);
    uint8_t StringToHex(QString string, uint8_t *pHex, uint16_t hexLen, uint16_t *pCHexNum);
    uint16_t StringToUint8(QString string, uint8_t *pData);
    void StrToHexStr(QString string, QString &stringHex);
    void Uint8ToString(uint8_t *pData, uint16_t dataLen, QString &string);
    void Uint8ToHex(uint8_t *pData, uint16_t dataLen, QString &string);
    void Warming(QString message);
    void OpenFirmFile(void);
};

#endif // MAINWINDOW_H

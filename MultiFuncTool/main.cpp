#include "mainwindow.h"
#include <QApplication>
#include "MySerialPor.h"
#include "comwarn.h"
#include "ProtocolClass.h"
#include "DobotProClass.h"
#include "NoneProClass.h"
#include "firmwareui.h"
#include "DobotMainClass.h"
#include "DobotV3MainClass.h"
#include "NoneMainClass.h"
#include "ProtocolLink.h"
/*************************************************************
** Function name:       main
** Descriptions:        主函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/

#ifdef Q_OS_WIN
#include <windows.h>
#include <thread>
#include <tlhelp32.h>
bool findPid(qint64 pid)
{
    bool bFind = false;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return bFind;
    }
    PROCESSENTRY32W pe32;
    memset(&pe32, 0, sizeof(PROCESSENTRY32W));
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    Process32FirstW(hSnapshot, &pe32);
    while (Process32NextW(hSnapshot, &pe32))
    {
        if (pid == pe32.th32ProcessID)
        {
            bFind = true;
            break;
        }
    }
    CloseHandle(hSnapshot);
    return bFind;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ComWarn comWarn;
    MySerialPor serial;
    FirmwareUI firmware;
    ProtocolClass_CONNECT(gProtocolClass, w, serial);
    ProtocolClass_CONNECT(gNoneProClass, w, serial);
    ProtocolClass_CONNECT(gDobotProClass, w, serial);
    UpdataClass_CONNECT(gDobotUpdataClass, w);
    ProtocolClass_CONNECT(gDobotV3ProClass, w, serial);
    UpdataClass_CONNECT(gDobotV3UpdataClass, w);
    QObject::connect(&w, SIGNAL(ComOpen(QString, qint32, QSerialPort::DataBits, QSerialPort::Parity, QSerialPort::StopBits)),
                     &serial, SLOT(Open(QString, qint32, QSerialPort::DataBits, QSerialPort::Parity, QSerialPort::StopBits)));
    QObject::connect(&w, SIGNAL(ComClose()),
                     &serial, SLOT(Close()));
    QObject::connect(&serial, SIGNAL(StatusSignal(bool)),
                     &w, SLOT(ComStatus(bool)));
    QObject::connect(&serial, SIGNAL(ComRefresh(QStringList)),
                     &w, SLOT(ComRefresh(QStringList)));
    QObject::connect(&serial, SIGNAL(ErrSignal(QString)),
                     &comWarn, SLOT(ErrShow(QString)));
    QObject::connect(&firmware, SIGNAL(ReturnFirmAddr(QString)),
                     &w, SLOT(ShowFirmAddr(QString)));
    DobotMainClass gDobotMainClass;
    gDobotMainClass.start();
    DobotV3MainClass gDobotV3MainClass;
    gDobotV3MainClass.start();
    NoneMainClass gNoneMainClass;
    gNoneMainClass.start();

    if (argc > 4)
    {
        QObject::connect(&w, SIGNAL(FirmwareShow(QString, QString)),
                         &firmware, SLOT(Show(QString, QString)));
        QObject::connect(&firmware, SIGNAL(startUpdate_signal()),
                         &w, SLOT(startUpdate_slot()));
        qDebug() << argv[2];
        QString url = QString::fromLocal8Bit(argv[1]);
        QString port = QString::fromLocal8Bit(argv[2]);
        QString version = QString::fromLocal8Bit(argv[3]);
        qint64 pid = QString::fromLocal8Bit(argv[4]).toLongLong();
#ifdef Q_OS_WIN
        if (pid != 0)
        {
            std::thread thd([pid]{
                while (findPid(pid))
                {//每间隔一段时间ms就检查一次pid是否存在，如果不存在整个应用退出
                    Sleep(200);
                }
                exit(0);
            });
            thd.detach();
        }
#endif
        serial.Open(port);
        w.BurnV3Firmware(url, version);
    }
    else if (argc > 3)
    {
        QObject::connect(&w, SIGNAL(FirmwareShow(QString, QString)),
                         &firmware, SLOT(Show(QString, QString)));
        QObject::connect(&firmware, SIGNAL(startUpdate_signal()),
                         &w, SLOT(startUpdate_slot()));
        qDebug() << argv[2];
        QString url = QString::fromLocal8Bit(argv[1]);
        QString port = QString::fromLocal8Bit(argv[2]);
        QString version = QString::fromLocal8Bit(argv[3]);
        serial.Open(port);
        w.BurnV3Firmware(url, version);
    }
    else
    {
        ;
        QObject::connect(&w, SIGNAL(FirmwareShow(QString)),
                         &firmware, SLOT(Show(QString)));
        w.show();
    }
    serial.start();
    return a.exec();
}

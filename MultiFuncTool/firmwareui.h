#ifndef FIRMWAREUI_H
#define FIRMWAREUI_H

#include <QDialog>
#include "QFile"
#include <QMessageBox>
#include "UpdataClass.h"

namespace Ui
{
    class FirmwareUI;
}

class FirmwareUI : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareUI(QWidget *parent = 0);
    ~FirmwareUI();
signals:
    void ReturnFirmAddr(QString fileAddr);
    void startUpdate_signal();

public slots:
    void Show(QString file);
    void Show(QString file, QString version);
private slots:
    void on_Sure_clicked();

private:
    Ui::FirmwareUI *ui;
    UpdataClass updataClass;
    QString dfirmFileAddr;
    void Warming(QString message);
    uint8_t OneAscallToHex(uint8_t ascall, uint8_t *pHex);
    uint8_t StringToHex(QString string, uint8_t *pHex, uint16_t hexLen, uint16_t *pCHexNum);
    uint16_t StringToUint8(QString string, uint8_t *pData);
    bool CheckFirmInfo(void);
};

#endif // FIRMWAREUI_H

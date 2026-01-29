/********************************************************************************
** Form generated from reading UI file 'firmwareui.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRMWAREUI_H
#define UI_FIRMWAREUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FirmwareUI
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *DeviceName;
    QLineEdit *DeviceAddr;
    QLineEdit *Version1;
    QLineEdit *InfoAddr;
    QLineEdit *ProAddr;
    QLineEdit *Version2;
    QLineEdit *Version3;
    QLineEdit *Version4;
    QLineEdit *Version5;
    QLineEdit *Version6;
    QLineEdit *Version7;
    QLineEdit *Version8;
    QPushButton *Sure;
    QPushButton *Cancel;
    QLabel *label_6;
    QLineEdit *TextFileAddr;

    void setupUi(QDialog *FirmwareUI)
    {
        if (FirmwareUI->objectName().isEmpty())
            FirmwareUI->setObjectName(QStringLiteral("FirmwareUI"));
        FirmwareUI->resize(460, 300);
        FirmwareUI->setMinimumSize(QSize(460, 300));
        FirmwareUI->setMaximumSize(QSize(460, 300));
        label = new QLabel(FirmwareUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 50, 101, 21));
        label_2 = new QLabel(FirmwareUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 170, 101, 21));
        label_3 = new QLabel(FirmwareUI);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 210, 101, 21));
        label_4 = new QLabel(FirmwareUI);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 90, 101, 21));
        label_5 = new QLabel(FirmwareUI);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 130, 101, 21));
        DeviceName = new QLineEdit(FirmwareUI);
        DeviceName->setObjectName(QStringLiteral("DeviceName"));
        DeviceName->setGeometry(QRect(160, 50, 231, 20));
        DeviceName->setMaxLength(50);
        DeviceAddr = new QLineEdit(FirmwareUI);
        DeviceAddr->setObjectName(QStringLiteral("DeviceAddr"));
        DeviceAddr->setGeometry(QRect(160, 90, 231, 20));
        DeviceAddr->setMaxLength(31278);
        Version1 = new QLineEdit(FirmwareUI);
        Version1->setObjectName(QStringLiteral("Version1"));
        Version1->setGeometry(QRect(160, 130, 21, 20));
        Version1->setMaxLength(2);
        Version1->setAlignment(Qt::AlignCenter);
        InfoAddr = new QLineEdit(FirmwareUI);
        InfoAddr->setObjectName(QStringLiteral("InfoAddr"));
        InfoAddr->setGeometry(QRect(160, 170, 231, 20));
        InfoAddr->setMaxLength(31278);
        ProAddr = new QLineEdit(FirmwareUI);
        ProAddr->setObjectName(QStringLiteral("ProAddr"));
        ProAddr->setGeometry(QRect(160, 210, 231, 20));
        ProAddr->setMaxLength(31278);
        Version2 = new QLineEdit(FirmwareUI);
        Version2->setObjectName(QStringLiteral("Version2"));
        Version2->setGeometry(QRect(190, 130, 21, 20));
        Version2->setMaxLength(2);
        Version2->setAlignment(Qt::AlignCenter);
        Version3 = new QLineEdit(FirmwareUI);
        Version3->setObjectName(QStringLiteral("Version3"));
        Version3->setGeometry(QRect(220, 130, 21, 20));
        Version3->setMaxLength(2);
        Version3->setAlignment(Qt::AlignCenter);
        Version4 = new QLineEdit(FirmwareUI);
        Version4->setObjectName(QStringLiteral("Version4"));
        Version4->setGeometry(QRect(250, 130, 21, 20));
        Version4->setMaxLength(2);
        Version4->setAlignment(Qt::AlignCenter);
        Version5 = new QLineEdit(FirmwareUI);
        Version5->setObjectName(QStringLiteral("Version5"));
        Version5->setGeometry(QRect(280, 130, 21, 20));
        Version5->setMaxLength(2);
        Version5->setAlignment(Qt::AlignCenter);
        Version6 = new QLineEdit(FirmwareUI);
        Version6->setObjectName(QStringLiteral("Version6"));
        Version6->setGeometry(QRect(310, 130, 21, 20));
        Version6->setMaxLength(2);
        Version6->setAlignment(Qt::AlignCenter);
        Version7 = new QLineEdit(FirmwareUI);
        Version7->setObjectName(QStringLiteral("Version7"));
        Version7->setGeometry(QRect(340, 130, 21, 20));
        Version7->setMaxLength(2);
        Version7->setAlignment(Qt::AlignCenter);
        Version8 = new QLineEdit(FirmwareUI);
        Version8->setObjectName(QStringLiteral("Version8"));
        Version8->setGeometry(QRect(370, 130, 21, 20));
        Version8->setMaxLength(2);
        Version8->setAlignment(Qt::AlignCenter);
        Sure = new QPushButton(FirmwareUI);
        Sure->setObjectName(QStringLiteral("Sure"));
        Sure->setGeometry(QRect(110, 260, 75, 23));
        Cancel = new QPushButton(FirmwareUI);
        Cancel->setObjectName(QStringLiteral("Cancel"));
        Cancel->setGeometry(QRect(270, 260, 75, 23));
        label_6 = new QLabel(FirmwareUI);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(50, 10, 101, 21));
        TextFileAddr = new QLineEdit(FirmwareUI);
        TextFileAddr->setObjectName(QStringLiteral("TextFileAddr"));
        TextFileAddr->setGeometry(QRect(160, 10, 231, 20));
        TextFileAddr->setMaxLength(32767);

        retranslateUi(FirmwareUI);
        QObject::connect(Cancel, SIGNAL(clicked()), FirmwareUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(FirmwareUI);
    } // setupUi

    void retranslateUi(QDialog *FirmwareUI)
    {
        FirmwareUI->setWindowTitle(QApplication::translate("FirmwareUI", "Dialog", nullptr));
        label->setText(QApplication::translate("FirmwareUI", "\350\256\276\345\244\207\345\220\215\347\247\260\357\274\232", nullptr));
        label_2->setText(QApplication::translate("FirmwareUI", "\344\277\241\346\201\257\346\256\265\350\265\267\345\247\213\345\234\260\345\235\200\357\274\232", nullptr));
        label_3->setText(QApplication::translate("FirmwareUI", "\347\250\213\345\272\217\346\256\265\350\265\267\345\247\213\345\234\260\345\235\200\357\274\232", nullptr));
        label_4->setText(QApplication::translate("FirmwareUI", "\350\256\276\345\244\207\345\234\260\345\235\200\357\274\232", nullptr));
        label_5->setText(QApplication::translate("FirmwareUI", "\345\233\272\344\273\266\347\211\210\346\234\254\345\217\267\357\274\232", nullptr));
        Sure->setText(QApplication::translate("FirmwareUI", "\347\241\256\345\256\232", nullptr));
        Cancel->setText(QApplication::translate("FirmwareUI", "\345\217\226\346\266\210", nullptr));
        label_6->setText(QApplication::translate("FirmwareUI", "\345\233\272\344\273\266\350\267\257\345\276\204\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FirmwareUI: public Ui_FirmwareUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRMWAREUI_H

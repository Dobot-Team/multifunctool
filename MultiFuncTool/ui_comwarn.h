/********************************************************************************
** Form generated from reading UI file 'comwarn.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMWARN_H
#define UI_COMWARN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ComWarn
{
public:
    QGridLayout *gridLayout;
    QPushButton *InfoButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QLabel *Info;

    void setupUi(QDialog *ComWarn)
    {
        if (ComWarn->objectName().isEmpty())
            ComWarn->setObjectName(QStringLiteral("ComWarn"));
        ComWarn->resize(200, 100);
        ComWarn->setMinimumSize(QSize(200, 100));
        ComWarn->setMaximumSize(QSize(200, 100));
        gridLayout = new QGridLayout(ComWarn);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        InfoButton = new QPushButton(ComWarn);
        InfoButton->setObjectName(QStringLiteral("InfoButton"));
        InfoButton->setMinimumSize(QSize(75, 23));
        InfoButton->setMaximumSize(QSize(75, 23));

        gridLayout->addWidget(InfoButton, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        Info = new QLabel(ComWarn);
        Info->setObjectName(QStringLiteral("Info"));
        Info->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(Info, 0, 0, 1, 3);


        retranslateUi(ComWarn);
        QObject::connect(InfoButton, SIGNAL(clicked()), ComWarn, SLOT(reject()));

        QMetaObject::connectSlotsByName(ComWarn);
    } // setupUi

    void retranslateUi(QDialog *ComWarn)
    {
        ComWarn->setWindowTitle(QApplication::translate("ComWarn", "Dialog", nullptr));
        InfoButton->setText(QString());
        Info->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ComWarn: public Ui_ComWarn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMWARN_H

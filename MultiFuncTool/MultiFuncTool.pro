#-------------------------------------------------
#
# Project created by QtCreator 2020-04-16T16:40:21
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiFuncTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        mainwindow.cpp \
        main.cpp \
        MySerialpor.cpp \
        comwarn.cpp \
        ../FrameWork/ProtocolFrame.c \
        ../FrameWork/ProtocolPacket.c \
        ../BaseClass/ProtocolClass.cpp \
        ../Function/None/NoneMainClass.cpp \
        ../Function/None/NoneProClass.cpp \
        firmwareui.cpp \
        ../FrameWork/md5.c \
        ../FrameWork/UpdataBase.c \
        ../FrameWork/UpdataMaster.c \
        ../BaseClass/UpdataClass.cpp \
        ../Function/Dobot/DobotFormat.cpp \
        ../Function/Dobot/DobotProClass.cpp \
        ../Function/Dobot/DobotMainClass.cpp \
        ../Function/Dobot/DobotUpdataClass.cpp \
        ../Function/DobotV3/DobotV3Format.cpp \
        ../Function/DobotV3/DobotV3ProClass.cpp \
        ../Function/DobotV3/DobotV3MainClass.cpp \
        ../Function/DobotV3/DobotV3UpdataClass.cpp \
        ../Function/ProtocolLink.cpp


HEADERS += \
        mainwindow.h \
        MySerialpor.h \
        comwarn.h \
        ../FrameWork/ProtocolFrame.h \
        ../FrameWork/ProtocolPacket.h \
        ../BaseClass/ProtocolClass.h \
        ../Function/None/NoneMainClass.h \
        ../Function/None/NoneProClass.h \
        firmwareui.h \
        ../FrameWork/md5.h \
        ../FrameWork/UpdataBase.h \
        ../FrameWork/UpdataMaster.h \
        ../BaseClass/UpdataClass.h \
        ../Function/Dobot/DobotFormat.h \
        ../Function/Dobot/DobotProClass.h \
        ../Function/Dobot/DobotMainClass.h \
        ../Function/Dobot/DobotUpdataClass.h \
        ../Function/DobotV3/DobotV3Format.h \
        ../Function/DobotV3/DobotV3ProClass.h \
        ../Function/DobotV3/DobotV3MainClass.h \
        ../Function/DobotV3/DobotV3UpdataClass.h \
        ../Function/ProtocolLink.h

FORMS += \
        mainwindow.ui \
        comwarn.ui \
        firmwareui.ui

INCLUDEPATH += \
            ../FrameWork \
            ../Function   \
            ../Function/Dobot \
            ../Function/DobotV3 \
            ../Function/None \
            ../BaseClass

RC_ICONS = MultiFuncTool.ico

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T21:50:35
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kfly_gui
TEMPLATE = app
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/kfly_comm/lib \
               $$PWD/kfly_comm/include

SOURCES += main.cpp \
           $$PWD/kfly_comm/src/kfly_comm.cpp \
           mainwindow.cpp \
           sliderbox.cpp \
           ui_connect.cpp

HEADERS  += mainwindow.h \
            sliderbox.h \
            ui_connect.h


FORMS    += mainwindow.ui \
            ui_connect.ui

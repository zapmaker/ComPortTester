#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T11:55:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComPortTester
TEMPLATE = app

include(QextSerialPort/qextserialport.pri)
include(log4qt/log4qt.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    rs232.cpp

HEADERS  += mainwindow.h \
    log4qtdef.h \
    rs232.h

FORMS    += mainwindow.ui

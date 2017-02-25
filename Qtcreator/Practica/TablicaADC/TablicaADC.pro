#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T11:49:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TablicaADC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialport.cpp \
    win_qextserialport.cpp

HEADERS  += mainwindow.h \
    qextserialenumerator.h \
    qextserialport.h \
    qextserialport_global.h

FORMS    += mainwindow.ui

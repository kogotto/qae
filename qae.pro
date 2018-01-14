#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T23:17:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qae
TEMPLATE = app
CONFIG += c++14


SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    mythread.h \
    model.h \
    work.h

FORMS    += mainwindow.ui

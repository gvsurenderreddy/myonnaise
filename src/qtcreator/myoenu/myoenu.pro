#-------------------------------------------------
#
# Project created by QtCreator 2014-09-20T04:21:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myoenu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    inputthread.cpp
    
QMAKE_CXXFLAGS += -std=c++11

HEADERS  += mainwindow.h \
    inputthread.h

FORMS    += mainwindow.ui

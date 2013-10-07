#-------------------------------------------------
#
# Project created by QtCreator 2013-07-23T16:54:51
#
#-------------------------------------------------

QT       += core gui

TARGET = WashingMachine
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
include(qextserialport/src/qextserialport.pri)

#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T22:16:14
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = objdetect
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp \
    rs232.cpp

HEADERS += \
    rs232.h

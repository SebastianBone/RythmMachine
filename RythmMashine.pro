#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T12:45:41
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = RythmMashine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mouseevent.cpp

HEADERS  += mainwindow.h \
    mouseevent.h

FORMS    += mainwindow.ui

include(opencv.pri)

#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T18:39:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RythmMachine
TEMPLATE = app


SOURCES += main.cpp\
    audioengine.cpp \
    dialog.cpp

HEADERS  += dialog.h \
    audioengine.h

FORMS    += dialog.ui

win32 {
INCLUDEPATH += C:\\opencv\\build\\include

LIBS += -LC:\\opencv\\build\\x86\\vc12\\lib \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_core249d \
    -lopencv_features2d249d \
    -lopencv_flann249d \
    -lopencv_gpu249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_legacy249d \
    -lopencv_ml249d \
    -lopencv_nonfree249d \
    -lopencv_objdetect249d \
    -lopencv_ocl249d \
    -lopencv_photo249d \
    -lopencv_stitching249d \
    -lopencv_superres249d \
    -lopencv_ts249d \
    -lopencv_video249d \
    -lopencv_videostab249d
}

linux* {

    LIBS += -L"/home/marko/Downloads/SFML-master-build/lib"

    CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
    CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

    INCLUDEPATH += "/home/marko/Downloads/SFML-master/include"
    DEPENDPATH += "/home/marko/Downloads/SFML-master/include"


    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

OTHER_FILES += \
    BD.wav \
    hit.mp3 \
    kick.mp3 \
    snare.wav \
    kick.wav \
    hit.wav

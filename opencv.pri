
win32 {
#  für Windows muss die Umgebungsvariable OPENCV_DIR gesetzt sein 
#  auf den Pfad der OpenCV-Installation
#  Beispiel: C:\OPENCV\opencv-2.4.9\build\x86\vc12

    INCLUDEPATH += $$(OPENCV_DIR)\..\..\include
    LIBS += -L$$(OPENCV_DIR)\lib
    Release:LIBS +=  -lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249
    Debug:LIBS +=  -lopencv_core249d -lopencv_highgui249d -lopencv_imgproc249d
}
macx {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
    LIBS +=  -lopencv_core -lopencv_highgui -lopencv_imgproc
}
linux* {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

#-------------------------------------------------
#
# Project created by QtCreator 2016-11-19T16:13:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = flowDepth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QCustomPlot/qcustomplot.cpp \
    common/base.cpp \
    common/test.cpp \
    flowThread/flow.cpp \
    depthThread/depthcaculate.cpp \
    depthThread/commondepth.cpp
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \
                /usr/include/python2.7

LIBS += /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_video.so \
        /usr/local/lib/libopencv_videostab.so \
        /usr/local/lib/libopencv_ml.so \
        /usr/local/lib/libopencv_calib3d.so \
        /usr/local/lib/libopencv_video.so \
        /usr/local/lib/libopencv_photo.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_imgcodecs.so

LIBS += -pthread
HEADERS  += mainwindow.h \
    QCustomPlot/qcustomplot.h \
    common/test.h \
    common/thread_cap.h \
    flowThread/opticalflow.h \
    depthThread/depthcaculate.h \
    depthparam.h

FORMS    += mainwindow.ui


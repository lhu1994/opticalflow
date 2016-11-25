QT += core
QT -= gui

TARGET = stereo_match
CONFIG += console
CONFIG -= app_bundle
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

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
TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    stereomatch.h \
    stereomatchparam.h


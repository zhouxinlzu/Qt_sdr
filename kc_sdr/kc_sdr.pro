#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T18:32:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kc_sdr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    engine/engine.cpp \
    spec/dockspec.cpp \
    spec/specbackground.cpp \
    spec/speccurve.cpp \
    spec/specwave.cpp \
    engine/libfft.cpp

HEADERS  += mainwindow.h \
    engine/engine.h \
    spec/dockspec.h \
    spec/specbackground.h \
    spec/speccurve.h \
    spec/specwave.h \
    engine/libfft.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

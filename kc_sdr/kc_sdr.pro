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
    dockspec.cpp \
    specwave.cpp \
    libfft.cpp \
    specbackground.cpp \
    speccurve.cpp \
    engine.cpp

HEADERS  += mainwindow.h \
    dockspec.h \
    specwave.h \
    libfft.h \
    specbackground.h \
    speccurve.h \
    engine.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

TEMPLATE = lib
CONFIG += c++14 staticlib

QT += qml quick core gui widgets

# Add -lqcustomplot if QCustomPlot is installed system-wide
LIBS += -lgiac -lgmp

INCLUDEPATH += ..
LIBS += ../keyboard/libkeyboard.a

SOURCES += \
    qcustomplot.cpp \
    customplotitem.cpp \
    curveitem.cpp

HEADERS += \
    qcustomplot.h \
    customplotitem.h \
    curveitem.h

TEMPLATE = app
TARGET = term
INCLUDEPATH += .

QT += core gui widgets
LIBS += -lqtermwidget5

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += term.cpp

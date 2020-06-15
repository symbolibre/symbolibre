TEMPLATE = app

QT += core gui printsupport qml quick widgets
CONFIG += c++14
INCLUDEPATH += ../../lib

LIBS += -L../../lib/ -lslmath \
        -lgiac -lgmp

SOURCES += main.cpp \

RESOURCES += qtquickcontrols2.conf

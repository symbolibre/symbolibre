TEMPLATE = lib
CONFIG  += staticlib

QT += qml quick
CONFIG += c++14
INCLUDEPATH += ../../editiontree

#LIBS += -L../../editiontree -leditiontree \
#        -lgiac -lgmp

SOURCES += keycode.cpp VirtualKeyboardContext.cpp

RESOURCES += qml.qrc

HEADERS += keycode.hpp VirtualKeyboardContext.hpp

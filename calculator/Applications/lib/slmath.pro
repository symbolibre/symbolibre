TEMPLATE = lib
CONFIG  += staticlib c++14
QT += qml quick

INCLUDEPATH += ../../editiontree
LIBS += -L../../editiontree -leditiontree

HEADERS  = FilesystemContext.hpp \
    MathContext.hpp \

SOURCES  = FileSystemContext.cpp \
    MathContext.cpp \

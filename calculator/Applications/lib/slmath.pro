TEMPLATE = lib
CONFIG  += staticlib c++14
QT += qml quick

INCLUDEPATH += ../../editiontree ../../../SLL
LIBS += -L../../editiontree -leditiontree \
    -L../../../SLL -lsll \

HEADERS  = FilesystemContext.hpp \
    MathContext.hpp \

SOURCES  = FileSystemContext.cpp \
    MathContext.cpp \

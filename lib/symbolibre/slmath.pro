TEMPLATE = lib
CONFIG  += c++14
QT += qml quick

INCLUDEPATH += editiontree
LIBS += -Wl,--whole-archive \
    ./editiontree/libeditiontree.a \
    ./graph/libgraph.a \
    ./keyboard/libkeyboard.a \
    -Wl,--no-whole-archive

HEADERS  = FilesystemContext.hpp \
    MathContext.hpp \
    editiontree.hpp

SOURCES  = FileSystemContext.cpp \
    MathContext.cpp

TEMPLATE = lib
CONFIG  += staticlib
LIBS += -lgiac
LIBS += -lgmp
CONFIG += c++11

SOURCES += Parser.cpp \
    Lexer.cpp \
    expression.cpp \
    interface.cpp

HEADERS += \
    Parser.h \
    Lexer.h \
    expression.hpp \
    interface.hpp
    


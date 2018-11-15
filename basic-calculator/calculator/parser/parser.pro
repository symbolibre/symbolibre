TEMPLATE = lib
CONFIG  += staticlib

CONFIG += c++11

SOURCES += Parser.cpp \
    Lexer.cpp \
    expression.cpp \
    interface.cpp

HEADERS += \
    Parser.h \
    Lexer.h \
    expression.h \
    interface.hpp
    


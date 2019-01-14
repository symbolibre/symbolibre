TEMPLATE = lib
CONFIG  += staticlib
CONFIG += c++11

SOURCES += Parser.cpp \
    Lexer.cpp \
    expression.cpp \
    ParserInterface.cpp \

HEADERS += \
    Parser.h \
    Lexer.h \
    expression.hpp \
    ParserInterface.hpp \    
    


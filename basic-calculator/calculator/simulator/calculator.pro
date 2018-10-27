TEMPLATE = app
QT += widgets
INCLUDEPATH += ../editiontree/
LIBS += -L../editiontree -lEditionTree

HEADERS       = button.hpp \
                fracrenderer.hpp \
                calculator.hpp 
SOURCES       = button.cpp \
				fracrenderer.cpp \
                calculator.cpp \
                main.cpp
 
TARGET = ../calculator
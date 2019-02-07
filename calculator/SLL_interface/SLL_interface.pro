TEMPLATE = lib
CONFIG  += staticlib
CONFIG += c++11

INCLUDEPATH += ../editiontree \
		../parser \

LIBS += -L../editiontree -leditiontree \
	-L../parser -lparser \
	
SOURCES += 
    interface.cpp \

HEADERS += 
    interface.hpp \

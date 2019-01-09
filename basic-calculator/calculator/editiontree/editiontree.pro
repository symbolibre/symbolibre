TEMPLATE = lib
CONFIG  += staticlib

LIBS += -lgiac
LIBS += -lgmp

HEADERS  = EditionArea.hpp \
       Operator.hpp \
		   Paren.hpp \ 
		   Frac.hpp \
		   Root.hpp \
		   Flow.hpp \
		   EditionNode.hpp \
		   InternalEditionNode.hpp \
		   ActiveEditionNodeVisitor.hpp \
		   visitors.hpp \
		   EditionTree.hpp

SOURCES  = EditionArea.cpp \
		   Operator.cpp \
		   Paren.cpp \
		   Root.cpp \
		   Frac.cpp \
		   Flow.cpp \
		   InternalEditionNode.cpp

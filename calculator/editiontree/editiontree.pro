TEMPLATE = lib
CONFIG  += staticlib		   
LIBS += -lgiac

HEADERS  = EditionArea.hpp \
		   Operator.hpp \
		   Paren.hpp \ 
		   Frac.hpp \
		   Root.hpp \
		   Power.hpp \
		   Sigma.hpp \
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
		   Power.cpp \
		   Sigma.cpp \
		   Flow.cpp \
		   fromGiacGen.cpp \
		   InternalEditionNode.cpp



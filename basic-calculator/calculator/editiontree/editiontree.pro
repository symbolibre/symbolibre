TEMPLATE = lib
CONFIG  += staticlib

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
		   EditionTree.hpp \
		   editiontrees.hpp

SOURCES  = EditionArea.cpp \
		   Operator.cpp \
		   Paren.cpp \
		   Root.cpp \
		   Frac.cpp \
		   Flow.cpp \
		   InternalEditionNode.cpp

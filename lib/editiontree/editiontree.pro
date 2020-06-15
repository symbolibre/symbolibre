TEMPLATE = lib
CONFIG  += staticlib c++14
QT += qml quick
LIBS += -lgiac -lgmp


HEADERS  = EditionArea.hpp \
		   Operator.hpp \
		   Variable.hpp \
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
		   Variable.cpp \
		   Paren.cpp \
		   Root.cpp \
		   Frac.cpp \
		   Power.cpp \
		   serialization.cpp \
		   Sigma.cpp \
		   Flow.cpp \
		   fromGiacGen.cpp \
		   InternalEditionNode.cpp \
		   ETBox.cpp 



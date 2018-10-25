QT += widgets

HEADERS       = button.hpp \
								EditionTree.hpp \ 
								Paren.hpp \
								Frac.hpp \
								EditionArea.hpp \
								Flow.hpp \
                fracrenderer.hpp \
                calculator.hpp 
SOURCES       = button.cpp \
								EditionArea.cpp \
								Paren.cpp \
								Frac.cpp \
								Flow.cpp \
								fracrenderer.cpp \
                calculator.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/calculator
INSTALLS += target

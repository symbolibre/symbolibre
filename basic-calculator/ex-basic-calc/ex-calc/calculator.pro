QT += widgets

HEADERS       = button.h \
								editiontree.h \ 
								paren.h \
								editionarea.h \
								flow.h \
                fracrenderer.h \
                calculator.h 
SOURCES       = button.cpp \
								editionarea.cpp \
								paren.cpp \
								flow.cpp \
								fracrenderer.cpp \
                calculator.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/calculator
INSTALLS += target

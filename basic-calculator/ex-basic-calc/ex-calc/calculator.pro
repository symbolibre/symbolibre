QT += widgets

HEADERS       = button.h \
								editiontree.h \ 
                fracrenderer.h \
                calculator.h 
SOURCES       = button.cpp \
								editiontree.cpp \
								fracrenderer.cpp \
                calculator.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/calculator
INSTALLS += target

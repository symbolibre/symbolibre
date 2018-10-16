QT += widgets

HEADERS       = prettyprinter.h \
								window.h
SOURCES       = main.cpp \
                prettyprinter.cpp \
                window.cpp

# install
target.path = ~/travail/M1IF/PI/qt-test/my-test-repr
INSTALLS += target

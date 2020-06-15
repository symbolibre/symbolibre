TEMPLATE = app

QT += qml quick core gui printsupport widgets
CONFIG += c++11
INCLUDEPATH += ../../lib

# Add -lqcustomplot if QCustomPlot is installed system-wide
LIBS += -L../../lib -lslmath \
	-lgiac -lgmp

SOURCES += main.cpp

RESOURCES += qtquickcontrols2.conf

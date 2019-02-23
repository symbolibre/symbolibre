QT += quick
QT += widgets
QT += quickcontrols2
#QT += KSyntaxHighlighting
CONFIG += c++17
cross_compile: DEFINES += QT_EXTRA_FILE_SELECTOR=\\\"touch\\\"
TARGET = ide.app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    documenthandler.cpp \
    languagesmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    documenthandler.h \
    languagesmodel.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/syntax-highlighting/bin/release/ -lKF5SyntaxHighlighting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/syntax-highlighting/bin/debug/ -lKF5SyntaxHighlighting
else:unix: LIBS += -L$$PWD/syntax-highlighting/bin/ -lKF5SyntaxHighlighting

INCLUDEPATH += $$PWD/syntax-highlighting/src/lib
DEPENDPATH += $$PWD/syntax-highlighting/src/lib

#include <QtPrintSupport/QPrinter>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <iostream>

#include "customplotitem.h"
#include "keycode.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");
    KeyCode::declareQML();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

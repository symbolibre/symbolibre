#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>

#include "calcsheet.hpp"
#include "keycode.hpp"
#include "customplotitem.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv); //QGuiApplication
    qmlRegisterType<CalcSheet>("org.symbolibre.mathrenderer", 1, 0, "MathRenderer");
    KeyCode::declareQML();
    CustomPlotItem::declareQML();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int ret_value = app.exec();
    std::cout << "quit\n";
    return ret_value;

}

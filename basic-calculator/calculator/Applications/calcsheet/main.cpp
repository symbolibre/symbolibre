#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>

#include "calcsheet.hpp"
#include "keycode.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CalcSheet>("org.symbolibre.mathrenderer", 1, 0, "MathRenderer");
    KeyCode::declareQML();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int ret_value = app.exec();
    std::cout << "quit\n";
    return ret_value;
}

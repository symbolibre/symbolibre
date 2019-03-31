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

    if (argc == 2) {
        if (strcmp(argv[1], "-keyboard") == 0) {
            engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
            if (engine.rootObjects().isEmpty()) {
                return -1;
            }
            int ret_value = app.exec();
            return ret_value;
        } else {
            printf("Use -keyboard option for having a keyboard\n");
            return (-1);
        }
    } else {
        engine.load(QUrl(QStringLiteral("qrc:/MainNoKeyboard.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

        int ret_value = app.exec();
        return ret_value;
    }
}

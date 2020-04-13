#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>

#include "keycode.hpp"
#include "customplotitem.h"
#include "EditionTree.hpp"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv); //QGuiApplication
    qmlRegisterType<ETBox>("etBox", 1, 0, "ETBox");
    KeyCode::declareQML();
    CustomPlotItem::declareQML();

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("./main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    int ret_value = app.exec();
    std::cout << "quit\n";
    return ret_value;

}

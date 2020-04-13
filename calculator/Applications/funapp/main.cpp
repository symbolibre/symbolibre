#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>

#include "FileSystemContext.hpp"
#include "MathContext.hpp"
#include "keycode.hpp"
#include "customplotitem.h"
#include "EditionTree.hpp"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv); //QGuiApplication
    qmlRegisterType<ETBox>("etBox", 1, 0, "ETBox");
    KeyCode::declareQML();
    CustomPlotItem::declareQML();

    FileSystemContext fs;
    MathContext math;

    QQmlEngine engine;
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("fs", &fs);
    context->setContextProperty("math", &math);

    QQmlComponent component(&engine, QUrl::fromLocalFile("./main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

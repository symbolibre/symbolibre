#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>

#include <symbolibre/config.hpp>
#include <symbolibre/FileSystemContext.hpp>
#include <symbolibre/MathContext.hpp>
#include <symbolibre/keyboard/keycode.hpp>


int main(int argc, char *argv[])
{

    QApplication app(argc, argv); //QGuiApplication
    KeyCode::declareQML();

    FileSystemContext fs;
    MathContext math;

    QQmlEngine engine;
    engine.addImportPath(SL_QML_DIR);
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("fs", &fs);
    context->setContextProperty("math", &math);

    QQmlComponent component(&engine, QUrl::fromLocalFile(QString(SL_QML_DIR) + "/funapp/main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

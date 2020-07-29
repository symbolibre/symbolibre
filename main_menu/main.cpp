#include "applauncher.h"

#include <symbolibre/config.hpp>

#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{
    AppLauncher launcher;

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("launcher", &launcher);

    engine.load(QUrl::fromLocalFile(QString(SL_QML_DIR) + "/menu/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

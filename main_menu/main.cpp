#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "applauncher.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AppLauncher launcher;
    engine.rootContext()->setContextProperty("launcher", &launcher);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

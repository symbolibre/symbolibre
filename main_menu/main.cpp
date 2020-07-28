#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDirIterator>
#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include "applauncher.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QDir appsDir("applications");
    QStringList apps(appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));
    QList<QObject*> appsModel;

    AppLauncher launcher;

    for(int i = 0; i < apps.size(); i++) {

        QFile file("applications/" + apps[i] + "/application.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        auto doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        auto appData = new AppItem; // FIXME leak
        appData->name = obj["caption"].toString();
        appData->path = obj["command"].toString().trimmed();
        QString icon = obj["icon"].toString().trimmed();
        if (icon[0] == '/') {
            appData->iconPath = "file:" + icon;
        } else {
            appData->iconPath = "file:applications/" + apps[i] + "/" + icon;
        }
        appsModel.append(appData);

        file.close();
    }

    engine.rootContext()->setContextProperty("launcher", &launcher);
    engine.rootContext()->setContextProperty("appsModel", QVariant::fromValue(appsModel));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

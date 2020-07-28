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

    QQmlComponent component(&engine, QUrl::fromLocalFile("JSONListModel.qml"));
    QObject* jsonListModel(component.create());

    QString config("[");

    QDir appsDir("applications");
    QStringList apps(appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));

    bool first = true;

    for(int i = 0; i < apps.size(); i++) {

        QFile file("applications/" + apps[i] + "/application.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        if(!first) {
            config += ",";
        } else {
            first = false;
        }

        QTextStream in(&file);
        QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
        QJsonObject obj = doc.object();
        QString command = obj["command"].toString().trimmed();
        obj["command"] = "cd applications/" + apps[i] + " && " + command + "";
        QString icon = obj["icon"].toString().trimmed();
        if (icon[0] == '/') {
            obj["icon"] = "file:" + icon;
        } else {
            obj["icon"] = "file:applications/" + apps[i] + "/" + icon;
        }
        doc.setObject(obj);
        config += QString::fromUtf8(doc.toJson());
        file.close();
    }
    config += "]";
    jsonListModel->setProperty("json", config);

    AppLauncher launcher;
    engine.rootContext()->setContextProperty("launcher", &launcher);
    QVariant listmodels(QVariant::fromValue(jsonListModel));
    engine.rootContext()->setContextProperty("listmodels", listmodels);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


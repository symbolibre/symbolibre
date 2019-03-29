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
#include "applauncher.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlComponent component(&engine, QUrl::fromLocalFile("JSONListModel.qml"));
    QObject *jsonListModel = component.create();

    QDirIterator it("applications", QDirIterator::Subdirectories);
    QRegExp rx("applications/*/application.json");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QString config = "[";
    bool first = true;

    std::vector<QString> apps;
    while (it.hasNext()) {
        QString path(it.next());
        if(rx.exactMatch(path)) {
            apps.push_back(path);
        }
    }



    std::sort(apps.begin(), apps.end());

    for(auto path : apps) {
        if(!first) {
            config += ",";
        } else {
            first = false;
        }

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;
        QTextStream in(&file);
        QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
        QJsonObject obj = doc.object();
        QString command = obj["command"].toString().trimmed();
        path.chop(17);
        obj["command"] = "cd " + path + " && " + command + "";
        QString icon = obj["icon"].toString().trimmed();
        if (icon[0] == '/') {
            obj["icon"] = "file:" + icon;
        } else {
            obj["icon"] = "file:" + path + "/" + icon;
        }
        doc.setObject(obj);
        config += QString::fromUtf8(doc.toJson());
        file.close();
    }


    config += "]";

    jsonListModel->setProperty("json", config);

    AppLauncher launcher;
    engine.rootContext()->setContextProperty("launcher", &launcher);
    engine.rootContext()->setContextProperty("listmodel", QVariant::fromValue(jsonListModel));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


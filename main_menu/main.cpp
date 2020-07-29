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
    QObject* jsonListModel[3] = {component.create(), component.create(), component.create()};

    QDirIterator it("applications", QDirIterator::Subdirectories);
    QRegExp rx("applications/*/application.json");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QString config[3] = {"[", "[", "["};


    std::vector<QString> apps;
    while (it.hasNext()) {
        QString path(it.next());
        if(rx.exactMatch(path)) {
            apps.push_back(path);
        }
    }
    std::sort(apps.begin(), apps.end());


    for(int page = 0; page<3; page++) {
        bool first = true;

        for(int i = 0; page==2 || i < 8; i++) {
            if(page*8+i >= apps.size()) break;

            QString &path = apps[page*8+i];

            if(!first) {
                config[page] += ",";
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
            config[page] += QString::fromUtf8(doc.toJson());
            file.close();
        }
        config[page] += "]";
        jsonListModel[page]->setProperty("json", config[page]);
    }




    AppLauncher launcher;
    engine.rootContext()->setContextProperty("launcher", &launcher);
    QList<QVariant> listmodels;
    for(int page = 0; page<3; page++) {
        listmodels.append(QVariant::fromValue(jsonListModel[page]));
    }
     engine.rootContext()->setContextProperty("listmodels", QVariant::fromValue(listmodels));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


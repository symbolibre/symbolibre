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
#include <QDebug>
#include <iostream>
#include "configrw.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlComponent component(&engine, QUrl::fromLocalFile("JSONListModel.qml"));
    QObject *jsonListModel = component.create();


    QFile file("layout.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "config.json not found" << std::endl;
        return 1;
    }

    QTextStream in(&file);
    //qDebug() << in.readAll();


    jsonListModel->setProperty("json", in.readAll().toUtf8());
    file.close();

    ConfigRW configrw("config.json");

    engine.rootContext()->setContextProperty("configrw", &configrw);
    engine.rootContext()->setContextProperty("configmodel", QVariant::fromValue(jsonListModel));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}

#include "configrw.h"
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


QJsonObject obj;
QString g_path;


ConfigRW::ConfigRW(QObject *parent) : QObject(parent)
{

}

ConfigRW::ConfigRW(QString path)
{
    g_path = path;
    QFile file_obj(path);
    if(!file_obj.open(QIODevice::ReadOnly)){
        qDebug()<<"Failed to open "<< path;
        exit(1);
    }

    QTextStream file_text(&file_obj);
    QString json_string;
    json_string = file_text.readAll();
    file_obj.close();
    QByteArray json_bytes = json_string.toLocal8Bit();

    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if(json_doc.isNull()){
        qDebug()<<"Failed to create JSON doc.";
        exit(2);
    }
    if(!json_doc.isObject()){
        qDebug()<<"JSON is not an object.";
        exit(3);
    }

    obj = json_doc.object();
}

QVariant ConfigRW::read(QString key)
{
    return obj[key].toVariant();
}

void ConfigRW::write(QString key, QVariant value)
{
    obj.insert(key, value.toJsonValue());
    QFile file(g_path);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonDocument doc(obj);
        stream << doc.toJson(QJsonDocument::Indented) << endl;
    }
}



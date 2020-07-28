#include "applauncher.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QStringList>

AppItem::AppItem(QObject *parent) : QObject(parent),
    id(), name(), path(), iconPath()
{

}

AppLauncher::AppLauncher(QObject *parent) : QObject(parent), mAppsModel()
{
    QDir appsDir("applications");
    QStringList apps(appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));

    for(int i = 0; i < apps.size(); i++) {

        QFile file("applications/" + apps[i] + "/application.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        auto doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        auto appData = new AppItem(this);
        appData->name = obj["caption"].toString();
        appData->path = obj["command"].toString().trimmed();
        QString icon = obj["icon"].toString().trimmed();
        if (icon[0] == '/') {
            appData->iconPath = "file:" + icon;
        } else {
            appData->iconPath = "file:applications/" + apps[i] + "/" + icon;
        }
        mAppsModel.append(appData);

        file.close();
    }
}

void AppLauncher::launch(QString command) {
    QProcess process;
    process.startDetached("/bin/sh", QStringList()<< "-c" << command);
}

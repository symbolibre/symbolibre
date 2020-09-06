#include "AppLauncher.hpp"

#include <symbolibre/config.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>

#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QStringList>

AppItem::AppItem(QObject *parent) : QObject(parent),
    id(), name(), executable(), applet(), iconPath()
{

}

AppLauncher::AppLauncher(QObject *parent) : QObject(parent), mAppsModel()
{
    QDir appsDir(Fs::staticDataDir() + "/applications");
    QStringList apps(appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));

    for(int i = 0; i < apps.size(); i++) {

        QFile file(appsDir.absoluteFilePath(apps[i] + "/application.json"));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "unable to read JSON data for application" << apps[i];
            continue;
        }

        auto doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        auto appData = new AppItem(this);
        appData->id = obj["id"].toString();
        if (appData->id.isEmpty())
            appData->id = apps[i];
        appData->name = obj["caption"].toString();
        appData->executable = obj["command"].toString().trimmed();
        appData->applet = obj["applet"].toString().trimmed();
        QString icon = obj["icon"].toString().trimmed();
        appData->iconPath = appsDir.absoluteFilePath(apps[i] + "/icon.png");
        if (QDir::isAbsolutePath(icon)) {
            appData->iconPath = icon;
        } else {
            appData->iconPath = appsDir.absoluteFilePath(apps[i] + "/" + icon);
        }
        mAppsModel.append(appData);

        file.close();
    }
}

const AppItem *AppLauncher::app(const QString &id) const
{
    for (auto obj : mAppsModel) {
        auto app = qobject_cast<const AppItem *>(obj);
        if (app->id == id)
            return app;
    }
    return nullptr;
}

bool AppLauncher::launch(AppItem *app)
{
    return QProcess::startDetached(app->executable, QStringList());
}

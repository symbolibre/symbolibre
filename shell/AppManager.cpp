// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AppManager.hpp"

#include <symbolibre/config.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>

#include <QCoreApplication>
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

AppManager::AppManager(QObject *parent) : QObject(parent), mAppsModel(), m_translator()
{
    QDir appsDir(Fs::staticDataDir() + "/apps");
    QStringList apps(appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));

    for(int i = 0; i < apps.size(); i++) {

        // TODO a 'hidden' boolean in the json would be more flexible
        if (apps[i] == "home")
            continue;

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

        // we look for a translated name in the json
        // FIXME this may not play well with country codes
        for (const auto &lang : QLocale().uiLanguages()) {
            auto name = obj["name_" + lang];
            if (name.isString()) {
                appData->name = name.toString();
                break;
            }
        }
        if (appData->name.isEmpty())
            appData->name = obj["name"].toString();

        appData->executable = obj["command"].toString().trimmed();
        appData->applet = obj["applet"].toString().trimmed();
        appData->iconPath = obj["icon"].toString().trimmed();
        mAppsModel.append(appData);

        file.close();
    }
}

const AppItem *AppManager::app(const QString &id) const
{
    for (auto obj : mAppsModel) {
        auto app = qobject_cast<const AppItem *>(obj);
        if (app->id == id)
            return app;
    }
    return nullptr;
}

void AppManager::loadAppletTranslation(AppItem *app)
{
    // QTranslator::load() discards the previous translation
    if (m_translator.load(QLocale(),
        Fs::staticDataDir() + "/apps/" + app->id + "/translations/" + app->id, "."))
        QCoreApplication::installTranslator(&m_translator);
}

bool AppManager::execute(AppItem *app)
{
    return QProcess::startDetached(app->executable, QStringList());
}

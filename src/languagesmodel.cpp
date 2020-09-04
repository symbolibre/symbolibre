#include "languagesmodel.h"

#include <iostream>
#include <QDebug>

LanguageData::LanguageData(QObject *parent) : QObject(parent)
{

}

LanguagesModel::LanguagesModel()
{
    QJsonObject json;

    QFile loadFile("ide.conf");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    else {

        QByteArray saveData = loadFile.readAll();

        QJsonDocument json(QJsonDocument::fromJson(saveData));

        QJsonArray lang_list = json.object()["languages"].toArray();

        for (int langIndex = 0; langIndex < lang_list.size(); ++langIndex) {
            QJsonObject language = lang_list[langIndex].toObject();

            const QString langName = language["name"].toString();
            auto *lang = new LanguageData(this);
            m_languages[langName] = lang;

            lang->name = langName;
            lang->command = language["cmd"].toString();
            lang->extension = language["extension"].toString();
        }
    }
    if (!m_languages.contains("Plain text")) {
        m_languages["Plain text"] = new LanguageData(this);
    }
}

// this should return a const reference, but QML does not allow it
LanguageData &LanguagesModel::getLanguageFromName(const QString langname)
{
    auto lang = m_languages.find(langname);
    if (lang != m_languages.end())
        return **lang;

    return *m_languages["Plain text"];
}

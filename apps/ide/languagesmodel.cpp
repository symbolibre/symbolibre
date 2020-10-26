#include "languagesmodel.hpp"

#include <iostream>
#include <QDebug>

#include <symbolibre/util/FileSystemSingleton.hpp>

LanguageData::LanguageData(QObject *parent) : QObject(parent)
{

}

LanguagesModel::LanguagesModel()
{
    QJsonObject json;

    QFile loadFile(Fs::staticDataDir() + "/ide/languages.json");

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
            lang->catalog = "";

            if (language.contains("catalog"))
                lang->catalog = language["catalog"].toString();
        }
    }
    if (!m_languages.contains("Plain text")) {
        m_languages["Plain text"] = new LanguageData(this);
    }
}

QStringList LanguagesModel::languages() const
{
    return m_languages.keys();
}

// This should return a const pointer, but QML does not allow it
LanguageData *LanguagesModel::data(const QString langname)
{
    auto lang = m_languages.find(langname);
    if (lang != m_languages.end())
        return *lang;

    return m_languages["Plain text"];
}

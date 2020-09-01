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

            auto snippets_dics = language["snippets"].toObject();

            for (auto snipp : snippets_dics.keys())
                lang->snippets[snipp] = snippets_dics[snipp].toString();

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

SnippetsModel::SnippetsModel()
{

}

QHash<int, QByteArray> SnippetsModel::roleNames() const
{
    return { { snippetKeyRole, "snippetKey" },
        { snippetTextRole, "snippetText" },
    };
}

int SnippetsModel::rowCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return 0;

    return m_snippets.size();
}

QVariant SnippetsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_snippets.size())
        return QVariant();

    QList<QString> keys = m_snippets.uniqueKeys();

    if (role == snippetKeyRole)  return keys[index.row()];
    else if (role == snippetTextRole)  return (m_snippets)[keys[index.row()]];
    else return QVariant();
}

snippetMap_t SnippetsModel::snippets()
{
    return m_snippets;
}

void SnippetsModel::setSnippets(snippetMap_t map)
{
    beginResetModel();
    m_snippets = map;

    endResetModel();
}

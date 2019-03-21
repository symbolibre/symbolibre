#include "languagesmodel.h"

#include <iostream>
#include <QDebug>

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
            LanguageItem lang;

            lang.languageName = language["name"].toString();
            lang.languageCmd = language["cmd"].toString();
            lang.languageColor = language["color_file"].toString();
            lang.languageExtension = language["extension"].toString();

            auto snippets_dics = language["snippets"].toObject();

            for (auto snipp : snippets_dics.keys())
                lang.snippets[snipp] = snippets_dics[snipp].toString();

            m_languageList.push_back(lang);
        }
    }
}

QHash<int, QByteArray> LanguagesModel::roleNames() const {
    return { { languageNameRole, "languageName" },
        { languageExtensionRole, "languageExtension" },
        { languageCmdRole, "languageCmd" },
    };
}

int LanguagesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_languageList.size();
}

bool LanguagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= m_languageList.size())
        return false;

    LanguageItem &language = m_languageList[index.row()];

    if (role == languageNameRole)  language.languageName = value.toString();
    else if (role == languageExtensionRole)  language.languageExtension = value.toString();
    else if (role == languageCmdRole)  language.languageCmd = value.toString();
    else return false;

    emit dataChanged(index, index, { role } );

    return true;
}

QVariant LanguagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_languageList.size())
        return QVariant();

    const LanguageItem &language = m_languageList.at(index.row());

    if (role == languageNameRole)  return language.languageName;
    else if (role == languageExtensionRole)  return language.languageExtension;
    else if (role == languageCmdRole)  return language.languageCmd;
    else return QVariant();
}

bool LanguagesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    for(int k = 0; k<count;k++)
    {
        LanguageItem language;
        language.languageCmd = QString("None");
        language.languageName = QString("None");
        language.languageExtension =  QString("None");

        m_languageList.insert(row + k, language);
    }
    endInsertRows();
    return true;
}

bool LanguagesModel::appendRow()
{
    return insertRows(m_languageList.size(), 1, QModelIndex());
}

QString LanguagesModel::getLine(int k)
{
    return m_languageList[k].languageName;
}

Qt::ItemFlags LanguagesModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEditable;
}

LanguageItem *LanguagesModel::getLanguageFromExtension(const QString extension)
{
    for(auto &v : this->m_languageList)
        if(v.languageExtension == extension)
            return &v;
    return nullptr;
}

LanguageItem *LanguagesModel::getLanguageFromName(const QString langname)
{
    for(auto &v : this->m_languageList)
        if(v.languageName == langname)
            return &v;
    return nullptr;
}

QString LanguagesModel::getExtensionFromId(const int idx)
{
    if (idx < 0)
        return m_languageList[0].languageExtension;

    return m_languageList[idx].languageExtension;
}

QString LanguagesModel::getColorationFromId(const int idx)
{
    if (idx < 0)
        return m_languageList[0].languageColor;
    return m_languageList[idx].languageColor;
}

int LanguagesModel::getIdFromExtension(const QString extension)
{
    QString extensionBis = extension;
    if (extensionBis[0] == QChar('.'))
            extensionBis.remove(0, 0); //extensionBis is the same extension with/without the starting dot
    else
        extensionBis.insert(0, QChar('.'));

    for(int i = 0; i<m_languageList.size(); i++){
        if(m_languageList[i].languageExtension == extension or m_languageList[i].languageExtension == extensionBis)
            return i;
    }

    return -1;
}

QString LanguagesModel::getCmdFromId(const int idx)
{
    if (idx < 0)
        return m_languageList[0].languageCmd;
    return m_languageList[idx].languageCmd;
}

snippetMap_t* LanguagesModel::getSnippetsFromId(const int idx)
{
    return &m_languageList[idx].snippets;
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
    if (m_snippets == nullptr)
    {
        return 0;
    }
    return m_snippets->size();
}

QVariant SnippetsModel::data(const QModelIndex &index, int role) const
{
    if(m_snippets == nullptr)
    {
        return QVariant();
    }
    if (index.row() >= m_snippets->size())
        return QVariant();

    QList<QString> keys = m_snippets->uniqueKeys();

    if (role == snippetKeyRole)  return keys[index.row()];
    else if (role == snippetTextRole)  return (*m_snippets)[keys[index.row()]];
    else return QVariant();
}

snippetMap_t* SnippetsModel::snippets()
{
    return m_snippets;
}

void SnippetsModel::setSnippets(snippetMap_t *map)
{
    m_snippets = map;
    emit dataChanged(QModelIndex(),QModelIndex());
}

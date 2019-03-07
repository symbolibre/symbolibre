#include "languagesmodel.h"

#include <iostream>

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
    return m_languageList[idx].languageExtension;
}

QString LanguagesModel::getColorationFromId(const int idx)
{
    return m_languageList[idx].languageColor;
}

int LanguagesModel::getIdFromExtension(const QString extension)
{
    for(int i = 0; i<m_languageList.size(); i++)
        if(m_languageList[i].languageExtension == extension)
            return i;

    return -1;
}

#include "languagesmodel.h"

LanguagesModel::LanguagesModel()
{

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

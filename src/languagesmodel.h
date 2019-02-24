#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <QAbstractListModel>

struct LanguageItem
{
    QString languageName;
    QString languageExtension;
    QString languageCmd;
    QString languageColor;
};

class LanguagesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)

public:
    LanguagesModel();

    enum Roles {languageNameRole, languageExtensionRole, languageCmdRole, languageColorRole};

    QHash<int,QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool appendRow();
    Q_INVOKABLE QString getLine(int k);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Utilities for the text editor
    Q_INVOKABLE LanguageItem* getLanguageFromExtension(const QString extension);
    Q_INVOKABLE LanguageItem* getLanguageFromName(const QString langname);

private:
    QVector<LanguageItem> m_languageList;
    int m_languageCount;
};

#endif // LANGUAGESMODEL_H

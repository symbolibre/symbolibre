#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMap>


typedef QMap<QString, QString> snippetMap_t;
struct LanguageItem
{
    QString languageName;
    QString languageExtension;
    QString languageCmd;
    QString languageColor;

    QMap<QString, QString> snippets;
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

    Q_INVOKABLE QString getExtensionFromId(const int idx);
    Q_INVOKABLE QString getColorationFromId(const int idx);
    Q_INVOKABLE int getIdFromExtension(const QString extension);
    Q_INVOKABLE QString getCmdFromId(const int idx);
    Q_INVOKABLE snippetMap_t* getSnippetsFromId(const int idx);

    QVector<LanguageItem> m_languageList;
private:
};

class SnippetsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)

    Q_PROPERTY(snippetMap_t* snippets READ snippets WRITE setSnippets NOTIFY snippetsChanged)
public:
    SnippetsModel();
    enum Roles {snippetKeyRole, snippetTextRole};

    QHash<int,QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    snippetMap_t* snippets();
    void setSnippets(snippetMap_t* map);


    QMap<QString, QString>* m_snippets;

Q_SIGNALS:
    void snippetsChanged();
private:
};


#endif // LANGUAGESMODEL_H

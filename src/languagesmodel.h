#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <memory>

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMap>


typedef QMap<QString, QString> snippetMap_t;

class LanguageData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString extension MEMBER extension CONSTANT)
    Q_PROPERTY(QString command MEMBER command CONSTANT)

    friend class LanguagesModel;

public:
    LanguageData(QObject *parent = nullptr);

    QString name;
    QString extension;
    QString command;
    QMap<QString, QString> snippets;
};

class LanguagesModel : public QObject
{
public:
    LanguagesModel();

    LanguageData &getLanguageFromName(const QString langname);

private:
    QMap<QString, LanguageData*> m_languages;
};

class SnippetsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)

    Q_PROPERTY(snippetMap_t snippets READ snippets WRITE setSnippets NOTIFY snippetsChanged)
public:
    SnippetsModel();
    enum Roles {snippetKeyRole, snippetTextRole};

    QHash<int,QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    snippetMap_t snippets();
    void setSnippets(snippetMap_t map);

    QMap<QString, QString> m_snippets;

Q_SIGNALS:
    void snippetsChanged();
private:
};


#endif // LANGUAGESMODEL_H

#ifndef LANGUAGESMODEL_HPP
#define LANGUAGESMODEL_HPP

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
};

class LanguagesModel : public QObject
{
public:
    LanguagesModel();

    LanguageData &getLanguageFromName(const QString langname);

private:
    QMap<QString, LanguageData*> m_languages;
};

#endif // LANGUAGESMODEL_HPP

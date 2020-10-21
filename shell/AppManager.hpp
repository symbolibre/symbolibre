// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SL_APPMANAGER_HPP
#define SL_APPMANAGER_HPP

#include <QObject>
#include <QTranslator>

class AppItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appId MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString executable MEMBER executable CONSTANT)
    Q_PROPERTY(QString applet MEMBER applet CONSTANT)
    Q_PROPERTY(QString iconPath MEMBER iconPath CONSTANT)

public:
    explicit AppItem(QObject *parent = nullptr);

    QString id;
    QString name;
    QString executable;
    QString applet;
    QString iconPath;
};

class AppManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> appsModel MEMBER mAppsModel CONSTANT)

public:
    explicit AppManager(QObject *parent = nullptr);

    const QList<QObject *> &apps() const { return mAppsModel; }
    const AppItem *app(const QString &id) const;
    Q_INVOKABLE void loadAppletTranslation(AppItem *app);
    Q_INVOKABLE bool execute(AppItem *app);

private:
    QList<QObject *> mAppsModel;
    QTranslator m_translator;
};

#endif // SL_APPMANAGER_HPP

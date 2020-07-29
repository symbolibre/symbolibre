#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <QObject>
#include <QProcess>


class AppItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString executable MEMBER executable CONSTANT)
    Q_PROPERTY(QString iconPath MEMBER iconPath CONSTANT)

public:
    explicit AppItem(QObject *parent = nullptr);

    QString id;
    QString name;
    QString executable;
    QString iconPath;
};

class AppLauncher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> appsModel MEMBER mAppsModel CONSTANT)

public:
    explicit AppLauncher(QObject *parent = nullptr);
    Q_INVOKABLE bool launch(AppItem *app);

private:
    QList<QObject*> mAppsModel;
};

#endif // APPLAUNCHER_H

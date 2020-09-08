#ifndef SL_MENU_APPLAUNCHER_HPP
#define SL_MENU_APPLAUNCHER_HPP

#include <QObject>
#include <QProcess>


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

class AppLauncher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> appsModel MEMBER mAppsModel CONSTANT)

public:
    explicit AppLauncher(QObject *parent = nullptr);

    const QList<QObject *> &apps() const { return mAppsModel; }
    const AppItem *app(const QString &id) const;
    Q_INVOKABLE bool launch(AppItem *app);

private:
    QList<QObject *> mAppsModel;
};

#endif // SL_MENU_APPLAUNCHER_HPP

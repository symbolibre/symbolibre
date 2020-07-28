#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <QObject>
#include <QProcess>


class AppItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString path MEMBER path CONSTANT)
    Q_PROPERTY(QString iconPath MEMBER iconPath CONSTANT)

public:
    QString id;
    QString name;
    QString path;
    QString iconPath;
};

class AppLauncher : public QObject
{
    Q_OBJECT
public:
    explicit AppLauncher(QObject *parent = nullptr);
    Q_INVOKABLE void launch(QString command);

signals:

public slots:
};

#endif // APPLAUNCHER_H

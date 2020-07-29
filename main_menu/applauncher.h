#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <QObject>
#include <QProcess>

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

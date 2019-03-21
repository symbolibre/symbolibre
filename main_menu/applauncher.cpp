#include "applauncher.h"
#include <QProcess>
#include <QDebug>

AppLauncher::AppLauncher(QObject *parent) : QObject(parent)
{

}

void AppLauncher::launch(QString command) {
    QProcess process;
    process.startDetached("/bin/sh", QStringList()<< "-c" << command);
}

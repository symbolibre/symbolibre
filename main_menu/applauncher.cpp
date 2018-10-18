#include "applauncher.h"
#include <QProcess>

AppLauncher::AppLauncher(QObject *parent) : QObject(parent)
{

}

void AppLauncher::launch(QString command) {
    QProcess process;
    process.startDetached(command);
}

#include "FileSystemSingleton.hpp"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

QString FileSystemSingleton::readFile(const QString &path) const
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    return QString::fromUtf8(file.readAll());
}

void FileSystemSingleton::writeFile(const QString &path, const QString &content) const
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
}

QString FileSystemSingleton::dataDir() const
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/symbolibre");
    dir.mkpath(".");
    return dir.absolutePath();
}

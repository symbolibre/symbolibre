#include "FileSystemContext.hpp"

#include <QFile>

QString FileSystemContext::readFile(const QString &path) const
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    return QString::fromUtf8(file.readAll());
}

void FileSystemContext::writeFile(const QString &path, const QString &content) const
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
}

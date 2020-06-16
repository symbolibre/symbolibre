#ifndef FILESYSTEMCONTEXT_HPP
#define FILESYSTEMCONTEXT_HPP

#include <QObject>
#include <QString>

class FileSystemContext : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE QString readFile(const QString &path) const;
    Q_INVOKABLE void writeFile(const QString &path, const QString &content) const;
    Q_INVOKABLE QString dataDir() const;
};

#endif // FILESYSTEMCONTEXT_HPP

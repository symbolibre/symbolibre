#ifndef FILESYSTEMSINGLETON_HPP
#define FILESYSTEMSINGLETON_HPP

#include <QObject>
#include <QQmlEngine>
#include <QString>

class FileSystemSingleton : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    Q_INVOKABLE QString readFile(const QString &path) const;
    Q_INVOKABLE void writeFile(const QString &path, const QString &content) const;

    Q_INVOKABLE QString staticDataDir() const;
    Q_INVOKABLE QString readWriteDataDir() const;
};

#endif // FILESYSTEMSINGLETON_HPP

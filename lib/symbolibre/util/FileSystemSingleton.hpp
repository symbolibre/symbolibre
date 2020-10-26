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
    Q_INVOKABLE static bool fileExists(const QString &path);
    Q_INVOKABLE static QString readFile(const QString &path);
    Q_INVOKABLE static void writeFile(const QString &path, const QString &content);
    Q_INVOKABLE static void deleteFile(const QString &path);

    Q_INVOKABLE static QString prefixDir();
    Q_INVOKABLE static QString qmlDir();
    Q_INVOKABLE static QString staticDataDir();
    Q_INVOKABLE static QString readWriteDataDir();
};

using Fs = FileSystemSingleton;

#endif // FILESYSTEMSINGLETON_HPP

#include "FileSystemSingleton.hpp"
#include "symbolibre/config.hpp"

#ifdef SL_TARGET_UNIX
# include <dlfcn.h>
#endif
#include <QDir>
#include <QFile>
#include <QStandardPaths>

bool FileSystemSingleton::fileExists(const QString &path)
{
    return QFile::exists(path);
}

QString FileSystemSingleton::readFile(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    return QString::fromUtf8(file.readAll());
}

void FileSystemSingleton::writeFile(const QString &path, const QString &content)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
}

QString FileSystemSingleton::prefixDir()
{
#ifdef SL_TARGET_UNIX
    Dl_info info;
    dladdr(reinterpret_cast<void*>(&FileSystemSingleton::prefixDir), &info);
    // this is the path to a (shared object) file, not a valid directory
    QDir prefix(info.dli_fname);
    // this gives the actual directory of the lib
    prefix.cdUp();
    // the prefix is the parent of the libdir
    prefix.cdUp();
    return prefix.absolutePath();
#else
    // FIXME we should probably use GetModuleFileName on Windows
    return SL_INSTALL_PREFIX;
#endif
}

QString FileSystemSingleton::qmlDir()
{
#ifdef SL_CUSTOM_QML_DIR
    return prefixDir() + "/" + SL_QML_DIR;
#else
    return SL_QML_DIR;
#endif
}

QString FileSystemSingleton::staticDataDir()
{
    return prefixDir() + "/" + SL_DATA_DIR;
}

QString FileSystemSingleton::readWriteDataDir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/symbolibre");
    dir.mkpath(".");
    return dir.absolutePath();
}

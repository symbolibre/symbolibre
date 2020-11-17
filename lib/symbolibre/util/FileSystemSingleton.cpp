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
    bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    return ok ? QString::fromUtf8(file.readAll()) : "";
}

void FileSystemSingleton::writeFile(const QString &path, const QString &content)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
}

void FileSystemSingleton::deleteFile(const QString &path)
{
    QFile file(path);
    file.remove();
}

bool FileSystemSingleton::createDir(const QString &path)
{
    QDir dir(path);
    return dir.mkpath(".");
}

bool FileSystemSingleton::copyFile(const QString &src, const QString &dst)
{
    return QFile(src).copy(dst);
}

QString FileSystemSingleton::prefixDir()
{
#ifdef SL_TARGET_UNIX
    Dl_info info;
    if (!dladdr(reinterpret_cast<void*>(&FileSystemSingleton::prefixDir), &info) ||
        !info.dli_fname) {
        qWarning() << "unable to locate libSymbolibre";
        return SL_INSTALL_PREFIX;
    }
    // this is the path to a (shared object) file, not a valid directory
    QDir prefix(info.dli_fname);
    // this gives the actual directory of the lib
    prefix.cdUp();
    // the relative path from the libdir to the prefix is known at build-time
    prefix.cd(SL_PREFIX_FROM_LIB_DIR);
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
    if (!dir.mkpath("."))
        qWarning() << "unable to create the read-write data directory";
    return dir.absolutePath();
}

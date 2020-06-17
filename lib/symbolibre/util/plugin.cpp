#include "FileSystemSingleton.hpp"
#include <QQmlExtensionPlugin>

class UtilQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.util")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.util"));
        qmlRegisterSingletonType<FileSystemSingleton>(uri, 1, 0, "Fs",
        [](QQmlEngine *, QJSEngine *) {
            return new FileSystemSingleton;
        }
                                                     );
    }
};

#include "plugin.moc"

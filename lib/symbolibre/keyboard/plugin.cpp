#include "keycode.hpp"
#include <QQmlExtensionPlugin>

class GraphQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.keyboard")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.keyboard"));
        qmlRegisterType<KeyCode>(uri, 1, 0, "KeyCode");
    }
};

#include "plugin.moc"

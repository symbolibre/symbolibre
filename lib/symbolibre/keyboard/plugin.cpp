#include "keycode.hpp"
#include "VirtualKeyboardContext.hpp"
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
        qmlRegisterType<VirtualKeyboardContext>("org.symbolibre.keyboard", 1, 0,
                                                "VirtualKeyboardContext");
    }
};

#include "plugin.moc"

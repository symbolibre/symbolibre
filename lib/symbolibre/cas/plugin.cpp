#include "MathContext.hpp"
#include <QQmlExtensionPlugin>

class CasQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.cas")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.cas"));
        qmlRegisterType<MathContext>("org.symbolibre.cas", 1, 0, "MathContext");
    }
};

#include "plugin.moc"

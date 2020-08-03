#include "SLQuickGraph.hpp"
#include <QQmlExtensionPlugin>

class GraphQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.graph")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.graph"));
        qmlRegisterType<SLQuickGraph>(uri, 1, 0, "GraphView");
    }
};

#include "plugin.moc"

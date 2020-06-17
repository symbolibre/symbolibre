#include "customplotitem.h"
#include <QQmlExtensionPlugin>

class GraphQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.graph")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.graph"));
        qmlRegisterType<CustomPlotItem>(uri, 1, 0, "GraphView");
    }
};

#include "plugin.moc"

#include "SLQuickExpr.hpp"
#include <QQmlExtensionPlugin>

class ExprQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.expr")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.expr"));
        qmlRegisterType<SLQuickExpr>(uri, 1, 0, "Expr");
    }
};

#include "plugin.moc"

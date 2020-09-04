#include "SourceEditor.h"
#include <QQmlExtensionPlugin>

class EditorQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.editor")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.editor"));
        qmlRegisterType<SourceEditor>(uri, 1, 0, "SourceEditor");
        qmlRegisterType<LanguageData>(uri, 1, 0, "LanguageData");
    }
};

#include "plugin.moc"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSyntaxHighlighter>
#include <QtQuickControls2>
#include <QQmlFileSelector>
#include <QApplication>

#include "documenthandler.h"
#include "languagesmodel.h"
#include "process.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<DocumentHandler>("DocumentHandler", 1, 0, "DocumentHandler");
    qmlRegisterType<LanguagesModel>("org.symbolibre.languagesModel", 1, 0, "LanguagesModel");
    qmlRegisterType<SnippetsModel>("org.symbolibre.snippetsModel", 1, 0, "SnippetsModel");
    qRegisterMetaType<snippetMap_t>("snippetMap_t");

    QStringList selectors;
#ifdef QT_EXTRA_FILE_SELECTOR
    selectors += QT_EXTRA_FILE_SELECTOR;
#else
    if (app.arguments().contains("-touch"))
        selectors += "touch";
#endif

    QQmlApplicationEngine engine;
    QQmlFileSelector::get(&engine)->setExtraSelectors(selectors);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

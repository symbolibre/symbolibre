#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>

#include <symbolibre/config.hpp>
#include <symbolibre/keyboard/VirtualKeyboardContext.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>
#include <symbolibre/applet/FunctionBarAttached.hpp>

#include "AppLauncher.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("symbolibre");

    QCommandLineParser parser;
    parser.addPositionalArgument("applet",
        QObject::tr("The applet to run (defaults to the main menu)"), "[applet]");
    parser.addOption({"list-apps", QObject::tr("List the installed applications and exit")});
    parser.addOption({{"k","keyboard"}, QObject::tr("Show the virtual keyboard")});
    parser.addHelpOption();
    parser.process(app);

    auto font(app.font());
    font.setPixelSize(12);
    font.setFamily("DejaVu Sans");
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setStyleStrategy(QFont::NoSubpixelAntialias);
    app.setFont(font);

    QPalette palette;
    palette.setColor(QPalette::Window,          "white");
    palette.setColor(QPalette::WindowText,      "black");
    palette.setColor(QPalette::Base,            "white");
    palette.setColor(QPalette::AlternateBase,   "lightgray");
    palette.setColor(QPalette::Text,            "black");
    palette.setColor(QPalette::ButtonText,      "white");
    palette.setColor(QPalette::BrightText,      "white");
    palette.setColor(QPalette::Highlight,       "#4080c0");
    palette.setColor(QPalette::HighlightedText, "white");
    palette.setColor(QPalette::Button,          "#2b79b5");
    palette.setColor(QPalette::Light,           "#0a67af");
    palette.setColor(QPalette::Midlight,        "#4e8fc1");
    palette.setColor(QPalette::Dark,            "#053d68");
    palette.setColor(QPalette::Mid,             "#074e84");
    app.setPalette(palette);

    QQuickStyle::setStyle(Fs::staticDataDir() + "/theme");

    QVariantMap initialProperties;

    AppLauncher launcher;
    VirtualKeyboardContext vk;
    vk.setActive(parser.isSet("keyboard"));

    if (parser.isSet("list-apps")) {
        for (auto *obj : launcher.apps()) {
            auto *app = qobject_cast<AppItem *>(obj);
            qInfo("%ls", qUtf16Printable(app->id));
        }
        return EXIT_SUCCESS;
    }
    if (parser.positionalArguments().size() == 1) {
        QString appletId = parser.positionalArguments().front();
        const auto *app = launcher.app(appletId);
        if (!app) {
            qCritical() << QObject::tr("Unknown application %1").arg(appletId);
            return EXIT_FAILURE;
        }
        if (app->applet.isEmpty()) {
            qCritical() << QObject::tr("Application %1 is not an applet").arg(appletId);
            return EXIT_FAILURE;
        }
        initialProperties.insert("initialApplet", app->applet);
    } else if (parser.positionalArguments().size() > 1) {
        qCritical() << QObject::tr("Too many positional arguments");
        return EXIT_FAILURE;
    } else {
        initialProperties.insert("initialApplet", QVariant());
    }

    QQmlEngine engine;
    engine.addImportPath(Fs::qmlDir());
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("launcher", &launcher);
    context->setContextProperty("keyboard", &vk);


    QQmlComponent component(&engine, QUrl::fromLocalFile(Fs::qmlDir() + "/shell/main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.createWithInitialProperties(initialProperties, context);
    context->setParent(window);

    auto w = qobject_cast<QQuickWindow *>(window);
    w->setTextRenderType(QQuickWindow::NativeTextRendering);

    return app.exec();
}

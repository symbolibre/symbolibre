#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <symbolibre/config.hpp>
#include <symbolibre/MathContext.hpp>
#include <symbolibre/keyboard/VirtualKeyboardContext.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>

#include "AppLauncher.hpp"

int main(int argc, char *argv[])
{
    using Fs = FileSystemSingleton;

    QApplication app(argc, argv);
    auto font(app.font());
    font.setPixelSize(13);
    app.setFont(font);

    AppLauncher launcher;
    VirtualKeyboardContext vk;
    MathContext math;

    QQuickStyle::setStyle(Fs::staticDataDir() + "/theme");

    QQmlEngine engine;
    engine.addImportPath(Fs::qmlDir());
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("launcher", &launcher);
    context->setContextProperty("keyboard", &vk);
    context->setContextProperty("math", &math);

    vk.setActive(argc >= 2 && strcmp(argv[1], "-keyboard") == 0);

    QQmlComponent component(&engine, QUrl::fromLocalFile(Fs::qmlDir() + "/menu/main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

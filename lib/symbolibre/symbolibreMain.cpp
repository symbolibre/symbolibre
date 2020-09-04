#include <QApplication>
#include <QFont>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickStyle>

#include <symbolibre/config.hpp>
#include <symbolibre/keyboard/VirtualKeyboardContext.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>

int symbolibreMain(int argc, char *argv[], QString componentPath)
{
    QApplication app(argc, argv);
    auto font(app.font());
    font.setPixelSize(13);
    app.setFont(font);

    VirtualKeyboardContext vk;

    QQuickStyle::setStyle(Fs::staticDataDir() + "/theme");

    QQmlEngine engine;
    engine.addImportPath(Fs::qmlDir());
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("keyboard", &vk);

    vk.setActive(argc >= 2 && strcmp(argv[1], "-keyboard") == 0);

    QQmlComponent component(&engine, QUrl::fromLocalFile(Fs::qmlDir() + "/" + componentPath));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

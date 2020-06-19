#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <symbolibre/config.hpp>
#include <symbolibre/MathContext.hpp>
#include <symbolibre/keyboard/VirtualKeyboardContext.hpp>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    auto font(app.font());
    font.setPixelSize(13);
    app.setFont(font);

    VirtualKeyboardContext vk;
    MathContext math;

    QQmlEngine engine;
    engine.addImportPath(SL_QML_DIR);
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("keyboard", &vk);
    context->setContextProperty("math", &math);

    vk.setActive(argc >= 2 && strcmp(argv[1], "-keyboard") == 0);

    QQmlComponent component(&engine, QUrl::fromLocalFile(QString(SL_QML_DIR) + "/calcsheet/main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <iostream>

#include <symbolibre/config.hpp>
#include <symbolibre/MathContext.hpp>
#include <symbolibre/util/FileSystemSingleton.hpp>


int main(int argc, char *argv[])
{
    using Fs = FileSystemSingleton;

    QApplication app(argc, argv); //QGuiApplication
    auto font(app.font());
    font.setPixelSize(13);
    app.setFont(font);

    MathContext math;

    QQuickStyle::setStyle(Fs::staticDataDir() + "/theme");

    QQmlEngine engine;
    engine.addImportPath(Fs::qmlDir());
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("math", &math);

    QQmlComponent component(&engine, QUrl::fromLocalFile(Fs::qmlDir() + "/funapp/main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}

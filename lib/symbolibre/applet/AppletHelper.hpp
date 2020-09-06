#ifndef SL_APPLETHELPER_HPP
#define SL_APPLETHELPER_HPP

#include <QObject>

class QQuickWindow;
class FunctionBarAttached;

class AppletHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FunctionBarAttached *functionBarModel READ functionBarModel NOTIFY functionBarChanged)
    Q_PROPERTY(QQuickWindow *window MEMBER m_window WRITE setWindow)

public:
    AppletHelper();

    FunctionBarAttached *functionBarModel() const { return m_functionBar; }

public slots:
    void setWindow(QQuickWindow *window);

signals:
    void functionBarChanged(FunctionBarAttached *functionBar);
    void windowChanged(QQuickWindow *window);

private slots:
    void updateModel();

private:
    FunctionBarAttached *m_functionBar;
    QQuickWindow *m_window;
};

#endif // SL_APPLETHELPER_HPP

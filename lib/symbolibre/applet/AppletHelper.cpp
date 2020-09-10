#include "AppletHelper.hpp"

#include <QQuickItem>
#include <QQuickWindow>

#include "FunctionBarAttached.hpp"

AppletHelper::AppletHelper() : QObject(), m_functionBar(nullptr), m_window(nullptr)
{

}

void AppletHelper::setWindow(QQuickWindow *window)
{
    if (window == m_window)
        return;

    if (m_window)
        disconnect(m_window, &QQuickWindow::activeFocusItemChanged,
            this, &AppletHelper::updateModel);
    if (window)
        connect(window, &QQuickWindow::activeFocusItemChanged,
            this, &AppletHelper::updateModel);

    m_window = window;
    if (m_window) updateModel();
}

void AppletHelper::updateModel()
{
    if (!m_window) {
        if (m_functionBar) {
            m_functionBar = nullptr;
            emit functionBarChanged(nullptr);
        }
        return;
    }

    auto *focusItem = m_window->activeFocusItem();
    while (focusItem) {
        auto *obj = qmlAttachedPropertiesObject<FunctionBarAttached>(focusItem, false);
        auto *attached = qobject_cast<FunctionBarAttached *>(obj);
        if (attached) {
            if (m_functionBar != attached) {
                m_functionBar = attached;
                emit functionBarChanged(m_functionBar);
            }
            return;
        }
        focusItem = focusItem->parentItem();
    }

    if (!m_functionBar)
        return;
    m_functionBar = nullptr;
    emit functionBarChanged(m_functionBar);
}

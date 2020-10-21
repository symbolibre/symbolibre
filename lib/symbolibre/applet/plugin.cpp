// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QQmlExtensionPlugin>

#include "AppletHelper.hpp"
#include "FunctionBarAttached.hpp"

class AppletQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.applet")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.applet"));
        qmlRegisterType<AppletHelper>("org.symbolibre.applet", 1, 0, "AppletHelper");
        qmlRegisterUncreatableType<FunctionBarAttached>("org.symbolibre.applet",
            1, 0, "FunctionBar", "FunctionBar is meant to be used as an attached property only");
        qmlRegisterType<FunctionKeyModel>("org.symbolibre.applet", 1, 0, "FunctionKeyModel");
    }
};

#include "plugin.moc"

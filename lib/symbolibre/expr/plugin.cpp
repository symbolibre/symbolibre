// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SLQuickExpr.hpp"
#include <QQmlExtensionPlugin>

class ExprQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.expr")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.expr"));
        qmlRegisterType<SLQuickExpr>(uri, 0, 2, "Expr");
    }
};

#include "plugin.moc"

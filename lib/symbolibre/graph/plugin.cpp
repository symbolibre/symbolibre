// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SLQuickGraph.hpp"
#include <QQmlExtensionPlugin>

class GraphQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.graph")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.graph"));
        qmlRegisterType<SLQuickGraph>(uri, 0, 2, "GraphView");
    }
};

#include "plugin.moc"

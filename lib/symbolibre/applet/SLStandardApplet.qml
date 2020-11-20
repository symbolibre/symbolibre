// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5

import org.symbolibre.applet 0.2

Page {
    id: root
    property var overlayParent: coveringItem
    property var windowTitle: undefined
    height: parent.height
    focus: true
    property var w: window

    AppletHelper {
        id: appletHelper
        window: root.w
    }

    footer: Rectangle {
        width: parent.width
        height: functionBar.height
        color: window.palette.mid

        SLFunctionBar {
            id: functionBar
            anchors.centerIn: parent
            barModel: appletHelper.functionBarModel
        }
    }

    Item {
        id: coveringItem
        anchors.fill: parent
        // This item needs to be above the application's main widget
        z: 10
    }

    // The ApplicationShortcut context is used to enable the shortcus in popups
    // TODO Try to use a repeater by making the model an array
    Shortcut {
        sequence: "F1"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barModel.f1 && functionBar.barModel.f1.activated()
    }
    Shortcut {
        sequence: "F2"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barModel.f2 && functionBar.barModel.f2.activated()
    }
    Shortcut {
        sequence: "F3"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barModel.f3 && functionBar.barModel.f3.activated()
    }
    Shortcut {
        sequence: "F4"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barModel.f4 && functionBar.barModel.f4.activated()
    }
    Shortcut {
        sequence: "F5"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barModel.f5 && functionBar.barModel.f5.activated()
    }
}

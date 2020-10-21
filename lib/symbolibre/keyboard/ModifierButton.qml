// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import org.symbolibre.keyboard 1.0
import org.symbolibre.style 1.0

Rectangle {
    id: root

    property int key
    property color backgroundColor: "#101010"
    property color textColor: "white"
    property bool pressed: false

    width: 60
    height: 30
    color: (tapHandler.pressed || root.pressed) ? "#404040" : backgroundColor
    radius: 6

    Text {
        color: textColor
        text: SLKey.label(root.key)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: SLStyle.largePixelSize
    }

    TapHandler {
        id: tapHandler
        onTapped: root.pressed = !root.pressed
    }
}

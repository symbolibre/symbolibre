// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import org.symbolibre.keyboard 0.2
import org.symbolibre.style 0.2

Rectangle {
    id: root

    property int mathKey
    property int progKey
    property int alphaKey
    property color backgroundColor: "#101010"
    readonly property bool alphaMode: keyboardRoot.alphaPressed && alphaKey
    readonly property bool progMode: progKey && !alphaMode && keyboardRoot.shiftPressed
    readonly property bool mathMode: !alphaMode && !progMode

    width: 60
    height: 30
    color: tapHandler.pressed ? "#404040" : backgroundColor
    radius: 6

    Text {
        id: mathText
        color: "white"
        text: mathKey ? SLKey.label(mathKey) : ""
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize:
            mathMode ? SLStyle.defaultPixelSize : SLStyle.smallPixelSize
        Behavior on font.pixelSize {
            NumberAnimation { duration: 200 }
        }
    }

    Text {
        id: progText
        color: "#a0c0ff"
        text: progKey ? SLKey.label(progKey) : ""
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 2
        font.pixelSize:
            progMode ? SLStyle.defaultPixelSize : SLStyle.smallPixelSize
        Behavior on font.pixelSize {
            NumberAnimation { duration: 200 }
        }
    }

    Text {
        id: alphaText
        color: "#ff4040"
        text: {
            alphaKey
                ? ((keyboardRoot.shiftPressed && alphaMode)
                    ? SLKey.label(alphaKey).toLowerCase()
                    : SLKey.label(alphaKey).toUpperCase())
                : ""
        }
        anchors.top: parent.top
        anchors.topMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 3
        horizontalAlignment: Text.AlignRight
        font.pixelSize:
            alphaMode ? SLStyle.defaultPixelSize : SLStyle.smallPixelSize
        Behavior on font.pixelSize {
            NumberAnimation { duration: 200 }
        }
    }

    TapHandler {
        id: tapHandler
        onTapped: {
            if (alphaMode && keyboardRoot.shiftPressed)
                keyboard.virtualKeyPress(alphaKey, 0, SLKey.text(alphaKey).toLowerCase());
            else if (alphaMode)
                keyboard.virtualKeyPress(alphaKey, Qt.ShiftModifier, SLKey.text(alphaKey));
            else if (progMode)
                keyboard.virtualKeyPress(progKey, 0, SLKey.text(progKey));
            else
                keyboard.virtualKeyPress(mathKey, 0, SLKey.text(mathKey));
        }
    }
}

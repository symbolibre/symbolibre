// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import org.symbolibre.style 0.2

Rectangle {
    property alias text: t.text
    property var model

    width: parent.width / 5
    height: parent.height
    color: model.color || window.palette.mid

    Text {
        id: t
        anchors.fill: parent
        font.pixelSize: SLStyle.smallPixelSize

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeight: 0.8
        color: "white"
        text: model.text || ""

        /* Alternative for bottom alignment:
        verticalAlignment: Text.AlignBottom
        bottomPadding: 1 */
    }

    MouseArea {
        anchors.fill: parent
        onPressed: model.activated()
    }
}

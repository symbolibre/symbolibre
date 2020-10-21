// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.style 1.0

Rectangle {
    width: parent.width
    height: 18

    color: window.palette.alternateBase
    property string label: "(Status Bar)"

    Row {
        Text {
            font.pixelSize: SLStyle.defaultPixelSize
            font.bold: true
            text: label

            topPadding: 1
            leftPadding: 2
        }
    }
}

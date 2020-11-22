// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0

Rectangle {
    width: 160
    height: 32
    color: row.activeFocus ? "lightblue" : "white"
    Text {
        width: 160
        height: 32
        id: element
        text: props.label
        font.pixelSize: 13
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}

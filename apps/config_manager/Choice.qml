// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.3

Row {
    focus: true
    property var props

    id: row
    width: 320
    height: 32
    spacing: 10
    ParamText {}

    ComboBox {
        id: combobox
        height: 32
        font.pixelSize: 13
        hoverEnabled: true
        flat: false
        width: 140
        model: props.choices
    }

    Keys.onLeftPressed: {
        if(combobox.currentIndex > 0) {
            combobox.currentIndex--
            configrw.write(props.key, combobox.currentIndex)
        }
    }

    Keys.onRightPressed: {
        if(combobox.currentIndex < combobox.count-1) {
            combobox.currentIndex++
            configrw.write(props.key, combobox.currentIndex)
        }
    }

    function init() {
        combobox.currentIndex = configrw.read(props.key)
    }
}













/*##^## Designer {
    D{i:0;height:24;width:320}D{i:2;anchors_height:16}
}
 ##^##*/

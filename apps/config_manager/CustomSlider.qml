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

    ParamText {}

    Slider {
        id: slider
        y: 8
        width: 160
        height: 16
        value: 0.5
    }

    Keys.onLeftPressed: {
        var lastvalue = slider.value
        slider.decrease()
        if(slider.value != lastvalue) {
            configrw.write(props.key, slider.value)
        }
    }

    Keys.onRightPressed: {
        var lastvalue = slider.value
        slider.increase()
        configrw.write(props.key, slider.value)
        if(slider.value != lastvalue) {
            configrw.write(props.key, slider.value)
        }
    }

    function init() {
        slider.value = configrw.read(props.key)
    }
}






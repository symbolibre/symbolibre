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

    Switch {
        id: onoff
        width: 160
        height: 32
        text: ""
    }

    Keys.onLeftPressed: {
        if(onoff.checked == true) {
            onoff.checked = false;
            configrw.write(props.key, false);
        }
    }

    Keys.onRightPressed: {
        if(onoff.checked == false) {
            onoff.checked = true;
            configrw.write(props.key, true);
        }
    }

    function init() {
        onoff.checked = configrw.read(props.key)
    }
}






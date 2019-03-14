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
        text: qsTr("")
    }

    Keys.onLeftPressed: {
        onoff.checked = false
    }

    Keys.onRightPressed: {
        onoff.checked = true
    }
}






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
        slider.decrease()
    }

    Keys.onRightPressed: {
        slider.increase()
    }
}






import QtQuick 2.12
import QtQuick.Controls 2.5

Row {
    height: 26

    property var colors: [
        window.palette.mid,
        window.palette.mid,
        window.palette.mid,
        window.palette.mid,
        window.palette.mid
    ]
    property var barmodel: undefined

    Repeater {
        model: 5
        SLFunctionKey {
            color: colors[index]
            text: barmodel[index]
        }
    }
}

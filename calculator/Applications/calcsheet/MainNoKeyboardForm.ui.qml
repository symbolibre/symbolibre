import QtQuick 2.8
import QtQuick.Controls 2.2
import SLKeyCode 1.0
import "../keyboard"

Rectangle {
    width: 320
    height: 220
    property alias formula: formula

    Rectangle {
        id: renderer
        height: parent.height
        width: 320
        x: 0
        y: 0
        color: "white"

        CalcSheet {
            id: formula
            height: parent.height
            width: 320
            x: 0
            y: 0
            focus: true
        }
    }
}

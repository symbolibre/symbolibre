import QtQuick 2.8
import QtQuick.Controls 2.2
import SLKeyCode 1.0
import "../keyboard"

Rectangle {
    width: 320
    height: 240
    property alias formula: formula

    Rectangle {
        id: renderer
        height: 240
        width: 320
        x: 0
        y: 0
        color: "white"

        CalcSheet {
            id: formula
            height: 240
            width: 320
            x: 0
            y: 0
            focus: true
        }
    }
}

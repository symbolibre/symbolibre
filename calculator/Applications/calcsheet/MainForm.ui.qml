import QtQuick 2.8
import QtQuick.Controls 2.2
import SLKeyCode 1.0
import "../keyboard"

Rectangle {
    width: 320
    height: 220 + 240
    property alias formula: formula
    property alias keyboard: keyboard

    Rectangle {
        id: renderer
        height: 220
        width: parent.width
        x: 0
        y: 0
        color: "white"

        CalcSheet {
            id: formula
            height: parent.height
            width: parent.width
            x: 0
            y: 0
            focus: true
        }
    }

    Rectangle {
        id: pad
        height: 240
        width: parent.width
        x: 0
        y: 220
        color: "black"

        SLKeyBoard {
            id: keyboard
        }
    }
}

import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import SLKeyCode 1.0
import "keyboard"

Window {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 240 : 0)
    title: qsTr("Quick Calculator")

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

    Loader {
        id: keyboardLoader
        source: keyboard.active ? "qrc:/keyboard/SLKeyBoard.qml" : ""
        x: 0
        y: 220
    }

    Connections {
        target: keyboardLoader.item
        onActivated: formula.recvInput(key)
    }
}

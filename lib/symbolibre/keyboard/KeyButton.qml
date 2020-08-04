import QtQuick 2.12
import org.symbolibre.keyboard 1.0

Item {
    id: root
    width: 60
    height: 30

    property int mathKey
    property int progKey
    property int alphaKey
    property color backgroundColor: "black"

    Rectangle {
        id: button
        color: tapHandler.pressed ? "#404040" : backgroundColor
        radius: 6
        anchors.fill: parent

        Text {
            id: mathText
            color: "white"
            text: mathKey ? SLKey.label(mathKey) : ""
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 13
        }

        Text {
            id: progText
            color: "lightblue"
            text: progKey ? SLKey.label(progKey) : ""
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 3
            font.pixelSize: 11
        }

        Text {
            id: alphaText
            color: "red"
            text: alphaKey ? SLKey.label(alphaKey) : ""
            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 2
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 11
        }

        TapHandler {
            id: tapHandler
            onTapped: keyboard.virtualKeyPress(mathKey, 0, "")
        }
    }
}

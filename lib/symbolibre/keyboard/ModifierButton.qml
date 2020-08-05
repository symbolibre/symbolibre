import QtQuick 2.12
import org.symbolibre.keyboard 1.0

Item {
    id: root
    width: 60
    height: 30

    property int key
    property color backgroundColor: "black"
    property color textColor: "white"
    property bool pressed: false

    Rectangle {
        id: button
        color: (tapHandler.pressed || root.pressed) ? "#404040" : backgroundColor
        radius: 6
        anchors.fill: parent

        Text {
            color: textColor
            text: SLKey.label(root.key)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 13
        }

        TapHandler {
            id: tapHandler
            onTapped: root.pressed = !root.pressed
        }
    }
}

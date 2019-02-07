import QtQuick 2.8

Item {
    id: root
    width: 30
    height: 30
    property alias mouseArea: mouseArea
    property alias mainTextText: mainText.text
    property color standardColor: "#ffff00"
    property color pressedColor: "#bbbb00"

    signal activated

    Rectangle {
        id: button
        color: standardColor
        radius: 8
        smooth: false
        antialiasing: false
        anchors.fill: parent

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onPressed: button.color = pressedColor
            onReleased: button.color = standardColor
        }

        Text {
            id: mainText
            x: 0
            y: 8
            width: 30
            height: 15
            text: qsTr("NULL")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
    }
}

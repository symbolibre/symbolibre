import QtQuick 2.8

Item {
    id: root
    width: 30
    height: 30
    property alias clickableArea: clickableArea
    property alias alphaTextText: alphaText.text
    property alias sndTextText: sndText.text
    property alias mainTextText: mainText.text
    property alias alphaTextStyleColor: alphaText.styleColor
    property color standardColor: "#56d94f"
    property color pressedColor: "#238d1b"

    signal activated

    Rectangle {
        id: button
        color: standardColor
        radius: 8
        smooth: false
        antialiasing: false
        anchors.fill: parent

        Text {
            id: mainText
            color: "#000000"
            text: "N"
            textFormat: Text.PlainText
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            font.family: "Ubuntu Sans"
            smooth: false
            antialiasing: false
            font.pixelSize: 11
        }

        Text {
            id: sndText
            color: "#0000ff"
            text: qsTr("2nd")
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 3
            textFormat: Text.AutoText
            font.family: "ubuntu sans mono"
            wrapMode: Text.NoWrap
            font.pixelSize: 8
        }

        Text {
            id: alphaText
            color: "#ff0000"
            text: qsTr("Alp")
            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 2
            elide: Text.ElideRight
            transformOrigin: Item.Center
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 8
            wrapMode: Text.NoWrap
            textFormat: Text.AutoText
            font.family: "ubuntu sans mono"
        }

        MouseArea {
            id: clickableArea
            anchors.fill: parent
            onPressed: button.color = pressedColor
            onReleased: button.color = standardColor
        }
    }
}
/*##^## Designer {
    D{i:2;anchors_x:11;anchors_y:16}D{i:4;anchors_y:3}
}
 ##^##*/


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
            x: 11
            y: 16
            color: "#000000"
            text: "N"
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
            horizontalAlignment: Text.AlignHCenter
            font.family: "Ubuntu Sans"
            smooth: false
            antialiasing: false
            font.pixelSize: 11
        }

        Text {
            id: sndText
            x: 2
            y: 3
            width: 13
            color: "#ffac04"
            text: qsTr("2nd")
            textFormat: Text.AutoText
            font.family: "ubuntu sans mono"
            wrapMode: Text.NoWrap
            font.pixelSize: 8
        }

        Text {
            id: alphaText
            x: 15
            y: 3
            width: 13
            height: 10
            color: "#ff0000"
            text: qsTr("Alp")
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

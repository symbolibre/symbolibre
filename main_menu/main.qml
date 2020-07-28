import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 320
    height: 220

    title: qsTr("Main Menu")

    Component {
        id: buttonDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight
            Column {
                anchors.fill: parent
                Image {
                    id: image
                    source: model.iconPath
                    anchors.horizontalCenter: parent.horizontalCenter
                    width:  48
                    height: width
                }
                Text {
                    id: caption
                    height: 26
                    text: model.name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 11
                    font.family: "DejaVu Sans Mono"
                    wrapMode: Text.WordWrap
                    width: grid.cellWidth
                    horizontalAlignment: Text.AlignHCenter
                    antialiasing: true
                }
            }
            Keys.onReturnPressed: launcher.launch(model.command)
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            color: "lightsteelblue"
            radius: 5
        }
    }

    GridView {
        id: grid
        anchors.rightMargin: 12
        anchors.bottomMargin: 24
        anchors.leftMargin: 12
        anchors.topMargin: 24
        anchors.fill: parent

        cellWidth: 74; cellHeight: 86

        property int page: 0

        model: appsModel
        delegate: buttonDelegate
        highlight: highlightBar
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 0

        focus: true
    }


    PageIndicator {
        id: control
        count: 3
        currentIndex: grid.page

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        delegate: Rectangle {
                implicitWidth: 10
                implicitHeight: 10

                radius: height/3
                color: index === control.currentIndex ? "black" : "lightgray"
        }
    }
}

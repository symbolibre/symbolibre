import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 320
    height: 216

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    title: qsTr("Main Menu")

    JSONListModel {
        id: testmodel
        source: "file:config.json"
    }

    Rectangle {
        width: Window.width
        height: Window.height

        Component {
            id: buttonDelegate
            Item {
                width: grid.cellWidth; height: grid.cellHeight
                Column {
                    anchors.fill: parent
                    Image {
                        source: model.icon
                        anchors.horizontalCenter: parent.horizontalCenter
                        width:  48
                        height: width
                    }
                    Text {
                        text: model.caption
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 16
                    }
                }
                Keys.onPressed: if (event.key === Qt.Key_Return) { launcher.launch(model.command) }
            }
        }

        GridView {
            id: grid
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            cellWidth: parent.width / 4 - 3; cellHeight: parent.height/3

            model: listmodel.model
            delegate: buttonDelegate
            highlight: Rectangle { color: "lightblue"; radius: 5 }
            focus: true

            ScrollBar.vertical: ScrollBar {
                id: vbar
                active: true
                orientation: Qt.Vertical
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                policy: ScrollBar.AlwaysOn
            }
        }
    }

}

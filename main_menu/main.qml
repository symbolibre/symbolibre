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
        id: listmodel
        source: "file:config.json"
    }

    Rectangle {
        width: Window.width
        height: Window.height

        Component {
            id: contactDelegate
            Item {
                width: grid.cellWidth; height: grid.cellHeight
                Column {
                    anchors.fill: parent
                    Image {
                        source: image
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 48
                        height: 48
                    }
                    Text {
                        text: caption
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 16
                    }
                }
                Keys.onPressed: if (event.key === Qt.Key_Return) { launcher.launch(command) }
            }
        }

        GridView {
            id: grid
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            cellWidth: 80; cellHeight: 72

            model: listmodel.model
            delegate: contactDelegate
            highlight: Rectangle { color: "lightblue"; radius: 5 }
            focus: true
        }
    }
}

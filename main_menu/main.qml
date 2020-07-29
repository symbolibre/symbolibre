import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import org.symbolibre.controls 1.0

SLWindow {
    visible: true
    width: 320
    height: 220

    title: qsTr("Symbolibre Menu")

    Dialog {
        id: launchErrorDialog
        anchors.centerIn: parent
        modal: true
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        contentItem: Text {
             text: qsTr("Unable to start the application")
        }
        onOpened: footer.standardButton(Dialog.Ok).forceActiveFocus()
    }

    Component {
        id: buttonDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight
            Column {
                anchors.fill: parent
                Image {
                    id: image
                    source: modelData.iconPath
                    anchors.horizontalCenter: parent.horizontalCenter
                    width:  48
                    height: width
                }
                Text {
                    id: caption
                    height: 26
                    text: modelData.name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 11
                    font.family: "DejaVu Sans Mono"
                    wrapMode: Text.WordWrap
                    width: grid.cellWidth
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    antialiasing: true
                }
            }
            Keys.onReturnPressed: if (!launcher.launch(modelData)) launchErrorDialog.open()
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            color: palette.highlight
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

        model: launcher.appsModel
        delegate: buttonDelegate
        highlight: highlightBar
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 0

        focus: true
    }
}

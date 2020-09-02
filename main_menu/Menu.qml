import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.controls 1.0
import org.symbolibre.keyboard 1.0

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

    // we intercept the home key here so that it does not open the dialog to back to the menu
    Keys.onPressed: if (event.key == SLKey.Home) event.accepted = true

    Dialog {
        id: launchErrorDialog
        anchors.centerIn: parent
        modal: true
        focus: true
        title: qsTr("Error")
        standardButtons: Dialog.Ok
        contentItem: Text {
            text: qsTr("Unable to start the application")
        }
        onClosed: grid.focus = true
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

            TapHandler {
                onPressedChanged: if (pressed) grid.currentIndex = index
                onTapped: launch()
            }

            Keys.onReturnPressed: launch()

            function launch() {
                if (modelData.applet)
                    appletLoader.setSource("../" + modelData.applet);
                else if (!launcher.launch(modelData))
                    launchErrorDialog.open();
            }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            color: palette.highlight
            radius: 5
        }
    }
}

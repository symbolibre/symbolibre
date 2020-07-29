import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import org.symbolibre.controls 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220

    title: qsTr("Symbolibre")

    Dialog {
        id: backToMenuDialog
        anchors.centerIn: parent
        modal: true
        title: qsTr("Quit")
        standardButtons: Dialog.Ok | Dialog.Cancel
        contentItem: Text {
            text: qsTr("Go back to the main menu?")
        }
        onOpened: footer.standardButton(Dialog.Cancel).forceActiveFocus()
        onAccepted: reloadMenu()
    }

    Item {
        id: mainItem
        anchors.fill: parent

        // FIXME why doesn't this work?
        /*Component {
           id: menuComponent
           Menu {

           }
        }*/

        Loader {
            id: appletLoader
            anchors.fill: parent
            focus: true
            source: "Menu.qml"
            onStatusChanged: {
                if (status == Loader.Error || status == Loader.Null) {
                    console.log("unable to load QML applet");
                    reloadMenu();
                    launchErrorDialog.open();
                }
            }
        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Home) {
                backToMenuDialog.open();
                event.accepted = true;
            }
        }
    }

    function reloadMenu() {
        appletLoader.source = "Menu.qml";
    }
}

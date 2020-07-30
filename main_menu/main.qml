import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import org.symbolibre.controls 1.0
import org.symbolibre.keyboard 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 240 : 0)

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

    ColumnLayout {
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
            Layout.fillWidth: true
            Layout.fillHeight: true
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

        Component {
            id: keyboardComponent
            SLKeyBoard {
            }
        }

        // the virtual keyboard is loaded lazily
        Loader {
            id: keyboardLoader
            Layout.fillWidth: true
            Layout.preferredHeight: keyboard.active ? 240 : 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            sourceComponent: keyboard.active ? keyboardComponent : null
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

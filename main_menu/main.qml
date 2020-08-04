import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.cas 1.0
import org.symbolibre.controls 1.0
import org.symbolibre.keyboard 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 320 : 0)

    title: qsTr("Symbolibre")

    MathContext {
        id: math
    }

    Dialog {
        id: backToMenuDialog
        anchors.centerIn: parent
        modal: true
        focus: true
        title: qsTr("Quit")
        standardButtons: Dialog.Ok | Dialog.Cancel
        contentItem: Text {
            text: qsTr("Go back to the main menu?")
        }
        onAccepted: reloadMenu()
    }

    Item {
        id: mainItem
        x: 0
        width: parent.width
        y: 0
        height: parent.height - (keyboard.active ? 320 : 0)

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

        Component {
            id: keyboardComponent
            Keyboard {
            }
        }


        Keys.onPressed: {
            if (event.key == Qt.Key_Home) {
                backToMenuDialog.open();
                event.accepted = true;
            }
        }
    }

    // the virtual keyboard is loaded lazily
    Loader {
        id: keyboardLoader
        parent: window.contentItem.parent // QQuickRootItem, parent of the overlay
        x: 0
        y: window.height - 320
        width: window.width
        height: 320
        z: 1000002 // the overlay has a z of 1000001
        sourceComponent: keyboard.active ? keyboardComponent : null
    }

    function reloadMenu() {
        appletLoader.source = "Menu.qml";
    }

    Connections {
        target: window.overlay
        function onHeightChanged(height) {
            updateOverlayHeight();
        }
    }

    Connections {
        target: mainItem
        function onHeightChanged(height) {
            updateOverlayHeight();
        }
    }

    function updateOverlayHeight() {
        window.overlay.height = mainItem.height;
    }
}

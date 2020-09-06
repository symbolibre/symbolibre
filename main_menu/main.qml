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
    height: 240 + (keyboard.active ? keyboardLoader.height : 0)
    title: qsTr("Symbolibre")

    required property string initialApplet

    // Move the overlay from the top-level window to the loaded application or
    // one of its subcomponents; this way, the status bar and keyboard are not
    // covered when opening popups.
    overlay.parent: appletLoader.item.overlayParent || appletLoader
    overlay.height: overlay.parent.height

    // The overlay is automatically resized with the window, so monitor changes
    // to make sure we always override this behavior
    Connections {
        target: overlay
        function onHeightChanged(height) {
            overlay.height = overlay.parent.height;
        }
        function onParentChanged(parent) {
            overlay.height = overlay.parent.height;
        }
    }
    Connections {
        target: window
        function onHeightChanged(height) {
            overlay.height = overlay.parent.height;
        }
    }

    MathContext {
        id: math
    }

    Dialog {
        id: backToMenuDialog
        anchors.centerIn: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("Quit")
        standardButtons: Dialog.Ok | Dialog.Cancel
        contentItem: Text {
            text: qsTr("Go back to the main menu?")
        }
        onAccepted: reloadMenu()
    }

    Component {
        id: keyboardComponent
        Keyboard {
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        SLStatusBar {
            id: statusBar
            Layout.fillWidth: true
            label: "Symbolibre"
        }

        Item {
            id: mainItem
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            Loader {
                id: appletLoader
                anchors.fill: parent
                focus: true
                source: window.initialApplet ? "../" + window.initialApplet : "Menu.qml"
                onStatusChanged: {
                    if (status == Loader.Error || status == Loader.Null) {
                        window.showError(qsTr("Unable to start the QML applet"));
                        reloadMenu();
                    }
                }
            }

            Keys.onPressed: {
                if (event.key == SLKey.Home) {
                    backToMenuDialog.open();
                    event.accepted = true;
                }
            }
        }

        // the virtual keyboard is loaded lazily
        Loader {
            id: keyboardLoader
            Layout.fillWidth: true
            Layout.preferredHeight: keyboard.active ? 320 : 0
            sourceComponent: keyboard.active ? keyboardComponent : null
        }
    }

    Shortcut {
        sequence: "Alt+K"
        onActivated: keyboard.active = !keyboard.active;
    }

    function reloadMenu() {
        appletLoader.source = "Menu.qml";
        statusBar.label = "Symbolibre";
    }

    function launch(app) {
        if (app.applet) {
            appletLoader.setSource("../" + app.applet);
            statusBar.label = app.name;
        } else if (!launcher.launch(app)) {
            window.showError(qsTr("Unable to start the application"));
        }
    }
}

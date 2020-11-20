// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.cas 0.2
import org.symbolibre.controls 0.2
import org.symbolibre.keyboard 0.2
import org.symbolibre.util 0.2

SLWindow {
    id: window
    visible: true
    width: 320
    height: 240 + (keyboard.active ? keyboardLoader.height : 0)
    title: appletLoader.item.windowTitle || qsTr("Symbolibre")

    required property QtObject appManager
    required property string initialApplet
    readonly property string appsDir: Fs.staticDataDir() + "/apps"

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
                source: appsDir + "/" + (window.initialApplet || "home/Menu.qml")
                onStatusChanged: {
                    if (status == Loader.Error || status == Loader.Null) {
                        reloadMenu();
                        window.showError(qsTr("Unable to start the QML applet"));
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
        appletLoader.source = appsDir + "/home/Menu.qml";
        statusBar.label = "Symbolibre";
    }

    function launch(app) {
        if (app.applet) {
            appletLoader.setSource(appsDir + "/" + app.applet);
            appManager.loadAppletTranslation(app);
            statusBar.label = app.name;
        } else if (!appManager.launch(app)) {
            window.showError(qsTr("Unable to start the application"));
        }
    }
}

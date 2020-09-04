import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    property alias functionBar: functionBar
    property var overlayParent: coveringItem
    height: parent.height
    focus: true

    footer: Rectangle {
        width: parent.width
        height: functionBar.height
        color: window.palette.mid

        SLFunctionBar {
            id: functionBar
            width: 320
            anchors.centerIn: parent

            barmodel: {
                var item = window.activeFocusItem;
                while (item) {
                    if (item.functionBarModel) {
                        return item.functionBarModel;
                    }
                    item = item.parent;
                }
            }
        }
    }

    Item {
        id: coveringItem
        anchors.fill: parent
        // This item needs to be above the application's main widget
        z: 10
    }

    // The ApplicationShortcut context is used to enable the shortcus in popups
    // TODO Try to use a repeater by making the model an array
    Shortcut {
        sequence: "F1"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barmodel.f1.activated()
    }
    Shortcut {
        sequence: "F2"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barmodel.f2.activated()
    }
    Shortcut {
        sequence: "F3"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barmodel.f3.activated()
    }
    Shortcut {
        sequence: "F4"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barmodel.f4.activated()
    }
    Shortcut {
        sequence: "F5"
        context: Qt.ApplicationShortcut
        onActivated: functionBar.barmodel.f5.activated()
    }
}

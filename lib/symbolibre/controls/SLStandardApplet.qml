import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    property alias functionBar: functionBar
    property var overlayParent: coveringItem
    height: parent.height
    focus: true

    footer: SLFunctionBar {
        id: functionBar
        width: parent.width

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
        onActivated:
            if(functionBar.barmodel.f1.action) functionBar.barmodel.f1.action();
    }
    Shortcut {
        sequence: "F2"
        context: Qt.ApplicationShortcut
        onActivated:
            if(functionBar.barmodel.f2.action) functionBar.barmodel.f2.action();
    }
    Shortcut {
        sequence: "F3"
        context: Qt.ApplicationShortcut
        onActivated:
            if(functionBar.barmodel.f3.action) functionBar.barmodel.f3.action();
    }
    Shortcut {
        sequence: "F4"
        context: Qt.ApplicationShortcut
        onActivated:
            if(functionBar.barmodel.f4.action) functionBar.barmodel.f4.action();
    }
    Shortcut {
        sequence: "F5"
        context: Qt.ApplicationShortcut
        onActivated:
            if(functionBar.barmodel.f5.action) functionBar.barmodel.f5.action();
    }

    // Also receive clicks on function keys
    Connections {
        target: functionBar
        function onActivated(action) {
            if(action) action();
        }
    }
}

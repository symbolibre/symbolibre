import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    property alias functionBar: functionBar
    property alias statusBar: statusBar
    focus: true

    footer: SLFunctionBar {
        id: functionBar
        parent: window.contentItem.parent
        z: 1000002

        barmodel: {
            var item = window.activeFocusItem;
            while (item) {
                if (item.functionBarModel) {
                    return item.functionBarModel;
                }
                item = item.parent;
            }
            return ["", "", "", "", ""];
        }
    }

    header: SLStatusBar {
        id: statusBar
        label: {
            var item = window.activeFocusItem;
            while (item) {
                if (item.statusBarLabel) {
                    return item.statusBarLabel;
                }
                item = item.parent;
            }
            return "(status)";
        }
    }
}

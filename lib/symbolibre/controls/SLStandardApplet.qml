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
}

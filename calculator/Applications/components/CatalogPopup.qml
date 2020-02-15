import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Popup {
    id: popup
    property var callback
    property alias currentMenu: catalog.currentMenu
    function setMenu(menu) {
        catalog.currentMenu = menu;
        catalog.reset();
    }

    modal: true
    focus: true
    visible: false
    padding: 1
    Catalog {
        id: catalog
        anchors.fill: parent
        keyNavigationWraps: true
        onEntrySelected: {
          callback(entry);
          popup.close();
        }
    }
}

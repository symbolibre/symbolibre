import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.controls 1.0

Popup {
    id: popup
    modal: true
    focus: true
    visible: false
    padding: 1
    property var callback
    property alias catalogId: catalog.catalogId
    property alias currentMenu: catalog.currentMenu
    function setMenu(menu) {
        catalog.currentMenu = menu;
        catalog.reset();
    }

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

// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.controls 0.2

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
        onEntrySelected: entry => {
          callback(entry);
          popup.close();
        }
    }
}

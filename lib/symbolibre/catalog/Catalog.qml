// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.XmlListModel 2.0
import org.symbolibre.util 0.2

RowLayout {
    id: root
    signal entrySelected(string entry)
    property string catalogId
    property bool translatable: true
    property string currentMenu: "root"
    property alias keyNavigationWraps: listView.keyNavigationWraps

    readonly property string translatedCatalogId: {
        if (!translatable)
            return catalogId;

        var path = (lang) => {
            // Check whether the directory with translated doc exists
            return Fs.staticDataDir() + "/catalog/" + catalogId + "_" + lang;
        };

        for (const lang of Qt.locale().uiLanguages) {
            if (Fs.fileExists(path(lang)))
                return catalogId + "_" + lang;

            var base = lang.substr(0,2);
            if (Fs.fileExists(path(base)))
                return catalogId + "_" + base;
        }
        return catalogId;
    }

    function reset() {
        listView.history = []
    }

    spacing: 0
    ListView {
        id: listView
        width: 120
        Layout.fillHeight: true
        focus: true

        property var history: []

        Loader {
            id: catalog_loader
            source: catalogId ? Fs.staticDataDir() + "/catalog/" + catalogId + ".qml" : "";
        }
        model: catalogId ? catalog_loader.item.get(currentMenu) : undefined

        delegate: ItemDelegate {
            width: parent.width
            text: (index < 10 ? String(index) + ". " : "   ") + model.title + (model.jump ? " ▸" : "")
            highlighted: ListView.isCurrentItem

            function confirm() {
                if (model.jump != undefined && model.jump != "") {
                    listView.history.push(currentMenu);
                    currentMenu = model.jump;
                } else if (model.insert != "") {
                    entrySelected(model.insert);
                } else {
                    entrySelected(model.title);
                }
            }
            function back() {
                if (listView.history.length) currentMenu = listView.history.pop()
            }
            onClicked: confirm()
            Keys.onRightPressed: confirm()
            Keys.onBackPressed: back()
            Keys.onLeftPressed: back()
        }
        Keys.onPressed: {
            var digit = event.key - Qt.Key_0;
            if (digit >= 0 && digit <= 9) {
                event.accepted = true;
                if (count >= digit + 1)
                    currentIndex = digit;
            }
        }
    }
    Rectangle {
        id: separator
        width: 1
        Layout.fillHeight: true
        color: palette.highlight
    }
    TextEdit {
        Layout.fillWidth: true
        Layout.fillHeight: true
        readOnly: true
        wrapMode: TextEdit.Wrap
        text: {
            if (catalog_loader.status != Loader.Ready || listView.currentIndex == -1)
                return "";
            var doc = listView.model.get(listView.currentIndex).doc;
            if (!doc)
                doc = listView.model.get(listView.currentIndex).title;
            getCatalogDoc(doc);
        }
        textFormat: TextEdit.RichText

        function getCatalogDoc(id) {
            return Fs.readFile(Fs.staticDataDir() + "/catalog/" +
                root.translatedCatalogId + "/" + String(id) + ".html");
        }
    }
}

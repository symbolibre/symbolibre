import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0
import QtQuick.XmlListModel 2.0
import org.symbolibre.config 1.0
import org.symbolibre.util 1.0

RowLayout {
    id: root
    signal entrySelected(string entry)
    property string catalogId
    property string currentMenu: "root"
    property alias keyNavigationWraps: listView.keyNavigationWraps

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

        model: XmlListModel {
            id: xmlModel
            source: SLConfig.dataDir + "/catalog/" + root.catalogId + ".xml"
            query: "/catalog/menu[@id=\"" + currentMenu + "\"]/entry"
            XmlRole { name: "title"; query: "./@title/string()" }
            XmlRole { name: "doc"; query: "./@doc/string()" }
            XmlRole { name: "insert"; query: "./string()" }
            XmlRole { name: "jump"; query: "./@jump/string()" }
            onStatusChanged: if (status == XmlListModel.Ready) listView.modelChanged()
        }
        delegate: ItemDelegate {
            width: parent.width
            text: (index < 10 ? String(index) + ". " : "   ") + model.title + (model.jump ? " >" : "")
            highlighted: ListView.isCurrentItem

            function confirm() {
                if (model.jump != "") {
                    listView.history.push(currentMenu);
                    currentMenu = model.jump;
                }
                if (model.insert != "") {
                    entrySelected(model.insert);
                }
            }
            function back() {
                if (listView.history.length) currentMenu = listView.history.pop()
            }
            onPressed: confirm()
            Keys.onReturnPressed: confirm()
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
            if (xmlModel.status != XmlListModel.Ready || listView.currentIndex == -1)
                return "";
            var doc = listView.model.get(listView.currentIndex).doc;
            if (!doc)
                doc = listView.model.get(listView.currentIndex).title;
            getCatalogDoc(doc);
        }
        textFormat: TextEdit.RichText

        function getCatalogDoc(id) {
            return Fs.readFile(SLConfig.dataDir + "/catalog/" + root.catalogId + "/" + String(id) + ".html");
        }
    }
}

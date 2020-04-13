import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Window 2.2
import SLKeyCode 1.0
import "keyboard"
import "components"

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 240 : 0)
    title: qsTr("Quick Calculator")

    ListModel {
        id: historyModel
        Component.onCompleted: {
            var json = JSON.parse(fs.readFile("./history.json"));
            for (var item of json)
                historyModel.append(item);
        }
        Component.onDestruction: {
            var json = [];
            for (let i = 0; i < historyModel.count; i++)
                json.push(historyModel.get(i));
            fs.writeFile("./history.json", JSON.stringify(json));
        }
    }

    CatalogPopup {
        id: catalog
        x: parent.x
        y: parent.y + parent.height / 6
        width: parent.width
        height: parent.height * 2 / 3
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

    ColumnLayout {
        anchors.fill: parent
        // FIXME find a way to align to bottom without reversing the model
        ListView {
            id: history
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            model: historyModel
            delegate: HistoryDelegate {}
            keyNavigationEnabled: true
            verticalLayoutDirection: ListView.BottomToTop
            onCurrentIndexChanged: {
                positionViewAtIndex(currentIndex, ListView.Contain);
                if (activeFocus && currentIndex == -1)
                    expr.forceActiveFocus();
            }
        }
        ExprField {
            id: expr
            focus: true
            Layout.fillWidth: true
            Layout.maximumHeight: 120
            Keys.onPressed: {
                if (event.key == Qt.Key_Return && text != "") {
                    historyModel.insert(0, {
                        "sourceJson": expr.json,
                        "resultJson": math.evaluate(expr.json, false)
                    });
                    expr.clear();
                    history.currentIndex = 0;
                    event.accepted = true;
                }
                if (event.key == Qt.Key_F1) {
                    window.openCatalog(function(str) {
                        expr.insertJson(str);
                    });
                    event.accepted = true;
                }
            }
            Component.onCompleted: forceActiveFocus()
            KeyNavigation.up: history.count ? history : null
        }
    }

    Loader {
        id: keyboardLoader
        source: keyboard.active ? "qrc:/keyboard/SLKeyBoard.qml" : ""
        x: 0
        y: 220
    }
}

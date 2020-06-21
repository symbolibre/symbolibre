import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.util 1.0
import org.symbolibre.keyboard 1.0
import org.symbolibre.controls 1.0
import org.symbolibre.catalog 1.0
import org.symbolibre.expr 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 240 : 0)
    title: qsTr("Quick Calculator")

    ListModel {
        id: historyModel
        Component.onCompleted: {
            var json = JSON.parse(Fs.readFile(Fs.readWriteDataDir() + "/history.json"));
            for (var item of json) {
                historyModel.append({
                    source: JSON.stringify(item.source),
                    result: JSON.stringify(item.result)
                });
            }
        }
        Component.onDestruction: {
            var json = [];
            for (let i = 0; i < historyModel.count; i++) {
                json.push({
                    source: JSON.parse(historyModel.get(i).source),
                    result: JSON.parse(historyModel.get(i).result)
                });
            }
            Fs.writeFile(Fs.readWriteDataDir() + "/history.json", JSON.stringify(json, null, 1));
        }
    }

    CatalogPopup {
        id: catalog
        x: parent.x
        y: parent.y + parent.height / 6
        width: parent.width
        height: parent.height * 2 / 3
        catalogId: "math_fr"
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
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
                        "source": expr.json,
                        "result": math.evaluate(expr.json, false)
                    });
                    expr.clear();
                    history.currentIndex = 0;
                    if (historyModel.count > 100)
                        historyModel.remove(historyModel.count - 1);
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

        // the virtual keyboard is loaded lazily
        Loader {
            id: keyboardLoader
            Layout.fillWidth: true
            Layout.preferredHeight: keyboard.active ? 240 : 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            sourceComponent: keyboard.active ? keyboardComponent : null
        }
    }

    Component {
        id: keyboardComponent
        SLKeyBoard {
        }
    }
}

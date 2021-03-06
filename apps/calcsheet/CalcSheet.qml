// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.applet 0.2
import org.symbolibre.util 0.2
import org.symbolibre.keyboard 0.2
import org.symbolibre.controls 0.2
import org.symbolibre.catalog 0.2

SLStandardApplet {
    id: app
    visible: true
    windowTitle: qsTr("Symbolibre Calculator")

    property bool approx: false

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
        anchors.centerIn: Overlay.overlay
        width: parent.width - 20
        height: parent.height - 20
        catalogId: "math"
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
            permanentEditFocus: true
            Layout.fillWidth: true
            Layout.maximumHeight: 120
            Keys.onPressed: {
                if (event.key == SLKey.Enter && text != "") {
                    historyModel.insert(0, {
                        "source": expr.json,
                        "result": math.evalExpr(expr.json, approx)
                    });
                    expr.clear();
                    history.currentIndex = 0;
                    if (historyModel.count > 100)
                        historyModel.remove(historyModel.count - 1);
                    event.accepted = true;
                }
            }
            Component.onCompleted: forceActiveFocus()
            KeyNavigation.up: history.count ? history : null

            FunctionBar.combine: true
            combineExprControls: true
        }
    }

    FunctionBar.f4: FunctionKeyModel {
        text: approx ? qsTr("Results:\nNumerical") : qsTr("Results:\nExact")
        onActivated: approx = !approx
    }
    FunctionBar.f5: FunctionKeyModel {
        text: math.angleMode ? qsTr("Angles:\nDegrees") : qsTr("Angles:\nRadians")
        onActivated: math.angleMode ^= 1
    }
}

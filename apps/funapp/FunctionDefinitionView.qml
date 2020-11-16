// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.util 0.2

FocusScope {
    property alias functions: fmodel
    ListModel {
        id: fmodel
        Component.onCompleted: {
            var data = Fs.readFile(Fs.readWriteDataDir() + "/functions.json");
            if (!data)
                data = Fs.readFile(Fs.staticDataDir() + "/funapp/functions_default.json");
            var json = JSON.parse(data);
            for (var item of json.functions) {
                item.expr = JSON.stringify(item.expr);
                fmodel.append(item);
            }
        }
        Component.onDestruction: {
            var json = {functions: []};
            for (let i = 0; i < fmodel.count; i++) {
                // get returns a QObject that does not support arbitrary assignments
                // so the deep copy turns it into a plain js object
                var f = JSON.parse(JSON.stringify(fmodel.get(i)));
                // f.expr is a JSON string stored in a js object about to be stringified
                // if we did not parse it here, it would end up stringified twice
                f.expr = JSON.parse(f.expr);
                json.functions.push(f);
            }
            Fs.writeFile(Fs.readWriteDataDir() + "/functions.json", JSON.stringify(json, null, 1));
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        ListView {
            id: fview
            spacing: 4
            focus: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: fmodel
            delegate: FunctionDefinitionDelegate { }
            keyNavigationEnabled: false
        }
    }
}

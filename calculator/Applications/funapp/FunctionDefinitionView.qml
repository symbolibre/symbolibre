import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


FocusScope {
    property alias functions: fmodel
    ListModel {
        id: fmodel
        Component.onCompleted: {
            var json = JSON.parse(fs.readFile("./functions.json"));
            if (!json.functions.length)
                json = JSON.parse(fs.readFile("./functions_default.json"))
            for (var item of json.functions)
                fmodel.append(item);
        }
        Component.onDestruction: {
            var json = {functions: []};
            for (let i = 0; i < fmodel.count; i++)
                json.functions.push(fmodel.get(i));
            fs.writeFile("./functions.json", JSON.stringify(json));
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

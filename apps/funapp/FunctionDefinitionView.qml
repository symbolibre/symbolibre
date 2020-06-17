import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import org.symbolibre.util 1.0

FocusScope {
    property alias functions: fmodel
    ListModel {
        id: fmodel
        Component.onCompleted: {
            var data = Fs.readFile(Fs.dataDir() + "/functions.json");
            if (!data)
                data = Fs.readFile(":/functions_default.json");
            var json = JSON.parse(data);
            for (var item of json.functions)
                fmodel.append(item);
        }
        Component.onDestruction: {
            var json = {functions: []};
            for (let i = 0; i < fmodel.count; i++)
                json.functions.push(fmodel.get(i));
            Fs.writeFile(Fs.dataDir() + "/functions.json", JSON.stringify(json));
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

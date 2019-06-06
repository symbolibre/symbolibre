import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


Item {
    property alias functions: fmodel
    ListModel {
        id: fmodel
        ListElement { name: "f1"; expr: "x+2"; color: "black" }
        ListElement { name: "f2"; expr: "x^2"; color: "red" }
        ListElement { name: "f3"; expr: "sin(x)"; color: "blue" }
        ListElement { name: "f4"; expr: "-x"; color: "orange" }
        ListElement { name: "f5"; expr: "-x"; color: "green" }
        ListElement { name: "f6"; expr: "-x"; color: "grey" }
    }

    ColumnLayout {
        anchors.fill: parent
        ListView {
            id: fview
            focus: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: fmodel
            delegate: FunctionDefinitionRow { focus: true }
        }
    }
}

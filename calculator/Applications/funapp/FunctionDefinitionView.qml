import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


FocusScope {
    property alias functions: fmodel
    ListModel {
        id: fmodel
        ListElement { active: false; name: "f1"; expr: "x+2"; color: "black" }
        ListElement { active: true; name: "f2"; expr: "x^2"; color: "red" }
        ListElement { active: true; name: "f3"; expr: "sin(x)"; color: "blue" }
        ListElement { active: true; name: "f4"; expr: "-x"; color: "orange" }
        ListElement { active: true; name: "f5"; expr: ""; color: "green" }
        ListElement { active: true; name: "f6"; expr: ""; color: "grey" }
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
            delegate: FunctionDefinitionRow { }
            keyNavigationEnabled: false
        }
    }
}

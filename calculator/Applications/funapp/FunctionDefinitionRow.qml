import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../components"

FocusScope {
    id: root
    focus: true
    width: parent.width
    height: exprfield.height
    RowLayout {
        id: row
        anchors.fill: parent
        CheckBox {
            id: enabled
            checked: model.active
            KeyNavigation.right: exprfield
            focus: true
        }
        Label {
            id: name
            text: model.name + "(x) = "
            color: model.color
            KeyNavigation.right: exprfield
        }
        SLTextField { // TODO ET
            id: exprfield
            Layout.fillWidth: true
            text: model.expr
            focus: true
        }
    }

    Binding {
        target: model
        property: "expr"
        value: exprfield.text
    }
    Binding {
        target: model
        property: "active"
        value: enabled.checked
    }
}

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

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
            KeyNavigation.right: exprfield
            focus: true
        }
        Label {
            id: name
            text: model.name + "(x) = "
            color: model.color
            KeyNavigation.right: exprfield
        }
        TextField { // TODO ET
            id: exprfield
            Layout.fillWidth: true
            text: model.expr
            focus: true
        }
    }
}

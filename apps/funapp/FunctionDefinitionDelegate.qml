import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.controls 1.0
import org.symbolibre.applet 1.0
import org.symbolibre.keyboard 1.0

FocusScope {
    property alias active: active
    property alias exprfield: exprfield

    id: root
    width: parent.width
    height: exprfield.height
    RowLayout {
        id: row
        anchors.fill: parent
        CheckBox {
            id: active
            checked: model.active
        }
        Label {
            id: name
            text: model.name + "(x) = "
            color: model.color
        }
        ExprField {
            id: exprfield
            Layout.fillWidth: true
            Layout.maximumHeight: 120
            json: model.expr
            onJsonChanged: model.expr = json
            focus: true
            Keys.onDownPressed: {
                root.ListView.view.incrementCurrentIndex();
                root.ListView.view.currentItem.exprfield.forceActiveFocus()
            }
            Keys.onUpPressed: {
                if (index == 0) {
                    event.accepted = false
                } else {
                    root.ListView.view.decrementCurrentIndex();
                    root.ListView.view.currentItem.exprfield.forceActiveFocus()
                }
            }

            FunctionBar.combine: true
            FunctionBar.f1: FunctionKeyModel {
                text: model.active ? qsTr("Hide") : qsTr("Show")
                onActivated: model.active = !model.active;
            }
        }
    }

    Binding {
        target: model
        property: "active"
        value: active.checked
    }
}

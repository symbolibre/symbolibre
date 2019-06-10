import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../components"

FocusScope {
    property alias active: active
    property alias exprfield: exprfield

    id: root
    focus: true
    width: parent.width
    height: exprfield.height
    RowLayout {
        id: row
        anchors.fill: parent
        SLCheckBox {
            id: active
            checked: model.active
            KeyNavigation.right: exprfield
            focus: true
            // FIXME is there a simpler/cleaner way to do this?
            Keys.onDownPressed: {
                root.ListView.view.incrementCurrentIndex();
                root.ListView.view.currentItem.active.forceActiveFocus()
            }
            Keys.onUpPressed: {
                if (index == 0) {
                    event.accepted = false
                } else {
                    root.ListView.view.decrementCurrentIndex();
                    root.ListView.view.currentItem.active.forceActiveFocus()
                }
            }
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
        value: active.checked
    }
}
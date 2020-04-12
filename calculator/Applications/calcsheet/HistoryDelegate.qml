import QtQuick 2.12
import QtQuick.Layouts 1.11
import "components"
import etBox 1.0

FocusScope {
    width: parent.width
    height: column.height
    ColumnLayout {
        id: column
        width: parent.width
        spacing: 0
        Rectangle {
            color: sourceExpr.activeFocus ? "lightblue" : "white"
            width: parent.width
            Layout.preferredHeight: sourceExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: sourceExpr
                width: parent.width
                height: parent.height
                horizontalAlignment: ETBox.AlignLeft
                json: model.sourceJson
                KeyNavigation.down: resultExpr
            }
        }

        Rectangle {
            color: resultExpr.activeFocus ? "lightblue" : "lightgray"
            width: parent.width
            Layout.preferredHeight: resultExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: resultExpr
                width: parent.width
                height: parent.height
                horizontalAlignment: ETBox.AlignRight
                json: model.resultJson
                focus: true
            }
        }
    }
}

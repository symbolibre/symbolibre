import QtQuick 2.12
import "components"
import etBox 1.0

FocusScope {
    width: parent.width
    height: column.height
    Column {
        id: column
        width: parent.width
        Rectangle {
            color: sourceExpr.activeFocus ? "lightblue" : "white"
            width: parent.width
            height: sourceExpr.height
            Expr {
                id: sourceExpr
                width: parent.width
                horizontalAlignment: ETBox.AlignLeft
                json: model.sourceJson
                KeyNavigation.down: resultExpr
            }
        }

        Rectangle {
            color: resultExpr.activeFocus ? "lightblue" : "lightgray"
            width: parent.width
            height: resultExpr.height
            Expr {
                id: resultExpr
                width: parent.width
                horizontalAlignment: ETBox.AlignRight
                json: model.resultJson
                focus: true
            }
        }
    }
}

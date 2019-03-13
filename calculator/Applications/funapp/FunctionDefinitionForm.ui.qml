import QtQuick 2.4
import "../components"

FocusDistributor {
    id: focusDistributor2
    width: 320
    height: 200

    property alias nameList : horizontalFocusDistributor.nameList
    property alias exprList : horizontalFocusDistributor.exprList

    HorizontalFocusDistributor {
        id: horizontalFocusDistributor
        anchors.fill: parent
        TextFieldCustom {
            id: textFieldCustom6
            y: 20
            width: 51
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 8
            transformOrigin: Item.Center
            initialText: "f(x)"
        }

        TextFieldCustom {
            id: textFieldCustom7
            y: 20
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 65
            anchors.right: parent.right
            anchors.rightMargin: 26
            initialText: ""
        }

        TextFieldCustom {
            id: textFieldCustom8
            y: 75
            width: 51
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 8
            initialText: "g(x)"
        }

        TextFieldCustom {
            id: textFieldCustom9
            y: 75
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 65
            anchors.right: parent.right
            anchors.rightMargin: 26
            initialText: ""
        }

        TextFieldCustom {
            id: textFieldCustom10
            y: 126
            width: 51
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 8
            initialText: "h(x)"
        }

        TextFieldCustom {
            id: textFieldCustom11
            y: 126
            height: 39
            anchors.left: parent.left
            anchors.leftMargin: 65
            anchors.right: parent.right
            anchors.rightMargin: 26
            initialText: ""
        }
    }
}

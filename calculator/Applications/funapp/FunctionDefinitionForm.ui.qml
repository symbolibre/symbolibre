import QtQuick 2.4
import "../components"

FocusDistributor {
    id: focusDistributor2
    width: 320
    height: 200

    property alias nameList: horizontalFocusDistributor.nameList
    property alias exprList: horizontalFocusDistributor.exprList

    property int vertical_spacing: height / 18 // Space between boxes
    property int horizontal_spacing: width / 8
    property int box_height: height / 5 // height of boxes
    property int name_width: width / 5
    property int expr_width: width / 2

    HorizontalFocusDistributor {
        id: horizontalFocusDistributor
        anchors.fill: parent
        TextFieldCustom {
            id: f
            y: 20
            width: name_width
            height: box_height
            text_prop: 3
            anchors.left: parent.left
            anchors.leftMargin: horizontal_spacing
            anchors.top: parent.top
            anchors.topMargin : vertical_spacing * 2
            transformOrigin: Item.Center
            initialText: "f(x)"
        }

        TextFieldCustom {
            id: fexpr
            y: 20
            height: box_height
            text_prop: 3
            width: expr_width
            anchors.rightMargin: horizontal_spacing
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin : vertical_spacing * 2
            initialText: ""
        }

        TextFieldCustom {
            id: g
            y: 75
            width: name_width
            height: box_height
            text_prop: 3
            anchors.left: parent.left
            anchors.leftMargin: horizontal_spacing
            anchors.top: f.bottom
            anchors.topMargin : vertical_spacing
            initialText: "g(x)"
        }

        TextFieldCustom {
            id: gexpr
            y: 75
            height: box_height
            text_prop: 3
            width: expr_width
            anchors.rightMargin: horizontal_spacing
            anchors.right: parent.right
            anchors.top: fexpr.bottom
            anchors.topMargin : vertical_spacing
            initialText: ""
        }

        TextFieldCustom {
            id: h
            y: 126
            width: name_width
            height: box_height
            text_prop: 3
            anchors.left: parent.left
            anchors.leftMargin: horizontal_spacing
            anchors.top: g.bottom
            anchors.topMargin : vertical_spacing
            initialText: "h(x)"
        }

        TextFieldCustom {
            id: hexpr
            y: 126
            height: box_height
            text_prop: 3
            width: expr_width
            anchors.rightMargin: horizontal_spacing
            anchors.right: parent.right
            anchors.top: gexpr.bottom
            anchors.topMargin : vertical_spacing
            initialText: ""
        }
    }
}

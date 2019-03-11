import QtQuick 2.0
import SLCustomPlotItem 1.0

Item {

    property var nameList
    property var exprList
    property int mode_int : 0

    function plot() {
        var name = ""
        var i = 0
        for(i=0; i < nameList.length; i++) {
            plotItem.addGraph(nameList[i] + "=" + exprList[i])
        }
    }

    id: graph
    property string mode : {
        if (mode_int == 1) {
            return "Cursor"
        }
        if (mode_int == 2) {
            return "Zoom"
        }
    }

    CustomPlotItem {
        id : plotItem
        anchors.fill: parent
    }

    Keys.onRightPressed: {
        if (mode == "Zoom") {
            plotItem.moveWindow(1, 0)
        }
        if (mode == "Cursor") {
            // TODO
        }
    }

    Keys.onLeftPressed: {
        if (mode == "Zoom") {
            plotItem.moveWindow(-1, 0)
        }
        if (mode == "Cursor") {
            // TODO
        }
    }
    Keys.onUpPressed: {
        if (mode == "Zoom") {
            plotItem.moveWindow(0, 1)
        }
        if (mode == "Cursor") {
            // TODO
        }
    }
    Keys.onDownPressed: {
        if (mode == "Zoom") {
            plotItem.moveWindow(0, -1)
        }
        if (mode == "Cursor") {
            // TODO
        }
    }

    Keys.onReturnPressed: {
        if (mode == "Zoom"){
            parent.forceActiveFocus()
        }
    }
}

import QtQuick 2.0
import SLCustomPlotItem 1.0

Item {

    property var nameList
    property var exprList
    property int mode_int : 0
    property int exit : 0

    CustomPlotItem { // Should not be aliased, the interface should be enought
        id : plotItem
        anchors.fill: parent
    }

    function plot() {
        var name = ""
        var i = 0
        for(i=0; i < nameList.length; i++) {
            plotItem.addGraph(nameList[i] + "=" + exprList[i])
        }
    }

    function setRange(xmin, xmax, ymin, ymax) {
        plotItem.setRange(xmin, xmax, ymin, ymax)
    }

    id: graph
    property string mode : {
        if (mode_int == 1) {
            return "Cursor"
        }
        if (mode_int == 2) {
            return "Zoom"
        }
        return "Def"
    }

    onFocusChanged: {
        if (focus == true) {
            exit = 0
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Right) {
            if (mode == "Zoom") {
                plotItem.moveWindow(1, 0)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Left) {
            if (mode == "Zoom") {
                plotItem.moveWindow(-1, 0)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Up) {
            if (mode == "Zoom") {
                plotItem.moveWindow(0, 1)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Down) {
            if (mode == "Zoom") {
                plotItem.moveWindow(0, -1)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Plus) {
            if (mode == "Zoom") {
                plotItem.modifyZoom(0.5)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Minus) {
            if (mode == "Zoom") {
                plotItem.modifyZoom(2.0)
                event.accepted = true
            }
            if (mode == "Cursor") {
                // TODO
                event.accepted = false
            }
        }

        else if (event.key === Qt.Key_Return) {
            if (mode == "Zoom") {
                exit = 1
                event.accepted = true
            }
        }

        else {
            event.accepted = false
        }
    }

}

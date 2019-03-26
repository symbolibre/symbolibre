import QtQuick 2.0
import SLCustomPlotItem 1.0

Item {

    property var nameList
    property var exprList
    property int mode_int : 0
    property int exit : 0

    signal rangeCom(double xmin, double xmax, double ymin, double ymax)
    // Send a signal saying that range changed. Send when initialized and when lose focus


    CustomPlotItem { // Should not be aliased, the interface should be enought
        id : plotItem
        anchors.fill: parent
    }

    function plot() {
        var name = ""
        var i = 0
        for(i=0; i < nameList.length; i++) {
            if (exprList[i] !== "") {
                plotItem.addGraph(nameList[i] + "=" + exprList[i])
            }
        }
        rangeCom(plotItem.getXmin(), plotItem.getXmax(), plotItem.getYmin(), plotItem.getXmax())
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

    onModeChanged: {
        if (mode == "Cursor") {
            plotItem.setModeCursor()
        }
        if (mode == "Zoom") {
            plotItem.setModeWindow()
        }
    }

    onFocusChanged: {
        if (focus == true) {
            exit = 0
        }
        else {
            rangeCom(plotItem.getXmin(), plotItem.getXmax(), plotItem.getYmin(), plotItem.getXmax())
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Right) {
            if (mode == "Zoom") {
                plotItem.moveWindow(1, 0)
                event.accepted = true
            }
            if (mode == "Cursor") {
                plotItem.moveCursor(1, 0)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Left) {
            if (mode == "Zoom") {
                plotItem.moveWindow(-1, 0)
                event.accepted = true
            }
            if (mode == "Cursor") {
                plotItem.moveCursor(-1, 0)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Up) {
            if (mode == "Zoom") {
                plotItem.moveWindow(0, 1)
                event.accepted = true
            }
            if (mode == "Cursor") {
                plotItem.moveCursor(0, 1)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Down) {
            if (mode == "Zoom") {
                plotItem.moveWindow(0, -1)
                event.accepted = true
            }
            if (mode == "Cursor") {
                plotItem.moveCursor(0, -1)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Plus) {
            if (mode == "Zoom" || mode == "Cursor") {
                plotItem.modifyZoom(0.5)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Minus) {
            if (mode == "Zoom" || mode == "Cursor") {
                plotItem.modifyZoom(2.0)
                event.accepted = true
            }
        }

        else if (event.key === Qt.Key_Return) {
            if (mode == "Zoom" || mode == "Cursor") {
                exit = 1
                event.accepted = true
            }
        }

        else {
            event.accepted = false
        }
    }

}

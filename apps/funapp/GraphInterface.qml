import QtQuick 2.0
import SLCustomPlotItem 1.0

Item {

    property ListModel functions
    signal exit

    property alias cursorPos : plotItem.cursorPos
    property alias view : plotItem.view
    property alias selectedCurve: plotItem.selectedCurve

    // Send a signal saying that range changed. Send when lose focus


    CustomPlotItem { // Should not be aliased, the interface should be enough
        id : plotItem
        anchors.fill: parent
    }

    function plot() {
        var i = 0
        for(i=0; i < functions.count; i++) {
            if (functions.get(i).expr !== "" && functions.get(i).active) {
                plotItem.addGraph(
                    functions.get(i).name + "(x)=" +
                    math.toGiac(functions.get(i).expr),
                    functions.get(i).color)
            }
        }
    }

    function setRange(xmin, xmax, ymin, ymax) {
        plotItem.setRange(Qt.rect(xmin, ymin, xmax - xmin, ymax - ymin))
    }

    id: graph
    property string mode : "Cursor"

    onModeChanged: {
        if (mode == "Cursor") {
            plotItem.setModeCursor()
        }
        if (mode == "Zoom") {
            plotItem.setModeWindow()
        }
    }

    onActiveFocusChanged: {
        if (!activeFocus) {
            viewChanged(plotItem.view)
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
                exit()
                event.accepted = true
            }
        }

        else {
            event.accepted = false
        }
    }

}

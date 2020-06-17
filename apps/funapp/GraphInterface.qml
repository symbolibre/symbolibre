import QtQuick 2.0
import org.symbolibre.graph 1.0

GraphView {
    id: plotItem

    property ListModel functions

    // Send a signal saying that range changed. Send when lose focus
    function plot() {
        var i = 0
        for(i=0; i < functions.count; i++) {
            if (functions.get(i).expr && functions.get(i).active) {
                plotItem.addGraph(
                    functions.get(i).name + "(x)=" +
                    math.toGiac(functions.get(i).expr),
                    functions.get(i).color)
            }
        }
    }

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

        else {
            event.accepted = false
        }
    }

}

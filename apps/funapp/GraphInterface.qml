import QtQuick 2.12
import org.symbolibre.graph 1.0
import org.symbolibre.keyboard 1.0

GraphView {
    id: plotItem

    property ListModel functions

    mathContext: math
    cursorAttached: true

    // Send a signal saying that range changed. Send when lose focus
    function plot() {
        plotItem.clearGraph();
        var i = 0
        for(i=0; i < functions.count; i++) {
            const f = functions.get(i);
            if (f.expr && f.active) {
                math.evalString(f.name + "(x):=" + math.toGiac(f.expr));
                plotItem.addGraph(f.name, f.color);
            }
        }
    }

    onActiveFocusChanged: {
        if (!activeFocus)
            viewChanged(plotItem.view);
    }

    Keys.onPressed: {
        event.accepted = true;

        if (event.key === Qt.Key_Right) {
            plotItem.moveCursor(1, 0);

        } else if (event.key === Qt.Key_Left) {
            plotItem.moveCursor(-1, 0);

        } else if (event.key === Qt.Key_Up) {
            plotItem.moveCursor(0, 1);

        } else if (event.key === Qt.Key_Down) {
            plotItem.moveCursor(0, -1);

        } else if (event.key === Qt.Key_Plus) {
            plotItem.zoomIn(2.);

        } else if (event.key === Qt.Key_Minus) {
            plotItem.zoomIn(.5);

        } else if (event.key === SLKey.F1) {
            plotItem.cursorAttached = !plotItem.cursorAttached;

        } else {
            event.accepted = false
        }
    }

}

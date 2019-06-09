import QtQuick 2.4

FunctionGraphForm {

    xtext.visible: graph.focus
    xtext.text: "X = " + String(graph.cursorX.toFixed(4))

    ytext.visible: graph.focus
    ytext.text: "Y = " + String(graph.cursorY.toFixed(4))

    mode.width: mode.parent.width

    graph.onExit: {
         mode.forceActiveFocus()
    }

    graph.onVisibleChanged: {
        if (graph.visible) {
            graph.setRange(menu.xmin, menu.xmax, menu.ymin, menu.ymax)
            graph.plot()
        }
    }

    Connections {
        target: graph
        onRangeCom : {
            menu.xmin = xmin
            menu.xmax = xmax
            menu.ymin = ymin
            menu.ymax = ymax
        }
    }
}

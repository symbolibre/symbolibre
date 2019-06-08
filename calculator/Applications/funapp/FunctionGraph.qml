import QtQuick 2.4

FunctionGraphForm {

    xtext.visible: graph.focus
    xtext.text: "X = " + String(graph.cursorX.toFixed(4))

    ytext.visible: graph.focus
    ytext.text: "Y = " + String(graph.cursorY.toFixed(4))

    mode.width: mode.parent.width

    graph.onExit: {
         focusedChild = focusedChild - 1
    }

    Connections {
        target: menu
        onDefined : {
            graph.setRange(xmin, xmax, ymin, ymax)
        }
    }



    /* TODO : maybe not really modular, but works anyway -\_("/)_/-
      Doing this in more general scope does not seem to work anyway */
    Connections {
        target: mainMenu
        onCurrentIndexChanged: {
            if (mainMenu.currentIndex == 1) {
                graph.plot()
            }
        }
    }

    Connections {
        target: graph
        onRangeCom : {
            menu.default_range(xmin, xmax, ymin, ymax)
        }
    }
}

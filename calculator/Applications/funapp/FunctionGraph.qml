import QtQuick 2.4

FunctionGraphForm {

    onExitGraphChanged: {
         if (exitGraph == 1) {
             focusedChild = focusedChild - 1
         }
    }

    stack.currentIndex: {
        if (mode.savedIndex == 0) {
            return 0
        }
        else {
            return 1
        }
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
        onSavedIndexChanged: {
            if (mainMenu.savedIndex == 1) {
                graph.plot()
            }
        }
    }
}

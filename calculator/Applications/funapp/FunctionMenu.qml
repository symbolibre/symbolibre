import QtQuick 2.4

FunctionMenuForm {
    onFocusChanged: {
        if (focus == true) {
            focusDistributor.forceActiveFocus()
        }
    }

    // When we press up at this point, we escape the menu
    Keys.onUpPressed: {
        event.accepted = false
        defined(xmin, xmax, ymin, ymax)
    }

    xmin: {
        return(parseFloat(xminEntry.text))
    }
    xmax: {
        return(parseFloat(xmaxEntry.text))
    }
    ymin: {
        return(parseFloat(yminEntry.text))
    }
    ymax: {
        return(parseFloat(ymaxEntry.text))
    }
}

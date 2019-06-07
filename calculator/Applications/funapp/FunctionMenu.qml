import QtQuick 2.4

FunctionMenuForm {
    onFocusChanged: {
        if (focus == true) {
            focusDistributor.forceActiveFocus()
        }
    }

    function default_range(xmin, xmax, ymin, ymax) {
        xminEntry.text = String(xmin.toFixed(2))
        xmaxEntry.text = String(xmax.toFixed(2))
        yminEntry.text = String(ymin.toFixed(2))
        ymaxEntry.text = String(ymax.toFixed(2))

    }

    // When we press up at this point, we escape the menu
    Keys.onUpPressed: {
        event.accepted = false
        defined(xmin, xmax, ymin, ymax)
    }

    xmin: parseFloat(xminEntry.text)
    xmax: parseFloat(xmaxEntry.text)
    ymin: parseFloat(yminEntry.text)
    ymax: parseFloat(ymaxEntry.text)
}

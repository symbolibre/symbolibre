import QtQuick 2.0

/* TODO : for now I dropped the idea of using focus scope.
  It would have been cleaner and click friendly on a theorical pow, but it was not necessary
  and it complicated things A LOT, especially on visual aspects, it was a terrible mess. */
FocusScope {

    /* property alias color: rectangle.color
    x: rectangle.x; y: rectangle.y
    width: rectangle.width; height: rectangle.height */
    // For quick copy pasting

    onFocusChanged: {
        if (focus == true) {
            children[0].forceActiveFocus()
        }
    }
}

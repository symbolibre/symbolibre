import QtQuick 2.4

SelectBarForm {
    focus: true
    Keys.onPressed: {
        if (event.key === Qt.Key_Right) {
            if (currentIndex < rectangle.children.length) {
                currentIndex = currentIndex + 1
            }
        }
        if (event.key === Qt.Key_Left) {
            if (currentIndex > 0) {
                currentIndex = currentIndex -1
            }
        }
        if (event.key === Qt.Key_Return) {
            savedIndex = currentIndex
        }
    }
}

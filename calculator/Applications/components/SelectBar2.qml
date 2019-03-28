import QtQuick 2.4

SelectBar2Form {
    focus: true
    Keys.onPressed: {
        if (event.key === Qt.Key_Right) {
            if (currentIndex < rectangle.children.length-1) {
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
    onCurrentIndexChanged: { // Modify to change behaviour
        savedIndex = currentIndex
    }
}

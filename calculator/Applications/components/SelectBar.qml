import QtQuick 2.4

SelectBarForm {
    Keys.onPressed: {
        if (event.key === Qt.Key_Right) {
            if (currentIndex < count-1) {
                incrementCurrentIndex()
            }
        }
        if (event.key === Qt.Key_Left) {
            if (currentIndex > 0) {
                decrementCurrentIndex()
            }
        }
    }
}

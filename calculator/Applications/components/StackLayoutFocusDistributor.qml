import QtQuick 2.0
import QtQuick.Layouts 1.3

StackLayout {
    currentIndex: selectBar.savedIndex

    onFocusChanged: { // When it gets focus: give focus to active child.
        children[currentIndex].forceActiveFocus()
    }
}

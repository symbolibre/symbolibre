import QtQuick 2.0
import QtQuick.Layouts 1.3

StackLayout {
    onActiveFocusChanged: { // When it gets focus: give focus to active child.
        children[currentIndex].forceActiveFocus()
    }
}

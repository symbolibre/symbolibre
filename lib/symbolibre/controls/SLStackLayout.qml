import QtQuick 2.12
import QtQuick.Layouts 1.12

StackLayout {
    onActiveFocusChanged: { // When it gets focus: give focus to active child.
        children[currentIndex].forceActiveFocus()
    }
}

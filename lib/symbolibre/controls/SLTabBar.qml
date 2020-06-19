import QtQuick 2.12
import QtQuick.Controls 2.5

TabBar {
    onActiveFocusChanged: {
        if (activeFocus)
            contentChildren[currentIndex].forceActiveFocus()
    }
}

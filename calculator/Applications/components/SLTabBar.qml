import QtQuick 2.4
import QtQuick.Controls 2.2

TabBar {
    onActiveFocusChanged: {
        if (activeFocus)
            contentChildren[currentIndex].forceActiveFocus()
    }
}

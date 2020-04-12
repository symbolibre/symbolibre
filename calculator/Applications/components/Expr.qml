import QtQuick 2.12
import QtQuick.Controls 2.2
import etBox 1.0

ETBox {
    id: etbox
    Keys.onPressed: {
        if(event.key === Qt.Key_Left)
            event.accepted = etbox.moveCursorLeft();
        if(event.key === Qt.Key_Right)
            event.accepted = etbox.moveCursorRight();
        if(event.key === Qt.Key_Up)
            event.accepted = etbox.moveCursorUp();
        if(event.key === Qt.Key_Down)
            event.accepted = etbox.moveCursorDown();
    }
    TapHandler {
        onTapped: forceActiveFocus()
    }
}


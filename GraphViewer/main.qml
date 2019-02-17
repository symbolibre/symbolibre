import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import CustomPlot 1.0
import SLKeyCode 1.0

Window {
    visible: true
    width: 320
    height: 240*2+30
    title: qsTr("Is this a graph ?")

    MainForm {
        anchors.fill: parent
        keyboard.onActivated: {
            plot.recvInput(key);
        }
        entree.onAccepted: {
            plot.addGraph(entree.text);
            entree.remove(0, entree.length)
        }
    }
}

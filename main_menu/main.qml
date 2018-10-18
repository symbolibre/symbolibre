import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.11

Window {
    visible: true
    width: 320
    height: 240
    title: qsTr("Main Menu")

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing : 20

        AppButton {
            caption: "gedit"
            command: "gedit"
        }
        AppButton {
            caption: "nautilus"
            command: "nautilus"
        }
        AppButton{
            caption: "firefox"
            command: "firefox"
        }
    }
}

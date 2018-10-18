import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.11

Button {
    property string caption: "Application name"
    property string command: ""
    id: button
    text: caption
    Layout.fillWidth: true
    Layout.fillHeight: true
    onClicked: launcher.launch(command);
}


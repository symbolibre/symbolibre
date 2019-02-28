import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

import org.symbolibre.languagesModel 1.0

Dialog {
    id: root
    visible: false
    width: 320
    height: 220

    TableView {
        id: tableView
        x: 0
        width: 320
        height: 174
        anchors.top: parent.top
        anchors.topMargin: 0

        TableViewColumn {
            role: "langageName"
            title: "Langage"
            width: 70
            delegate: delegateName
        }
        TableViewColumn {
            role: "languageCmd"
            title: "Cmd"
            width: 70
            delegate: delegateCmd
        }

        TableViewColumn {
            role: "languageExtension"
            title: "Extension"
            width: 100
            delegate: delegateExtension
        }

        model: langModel
    }

    Button {
        id: addLanguageButton
        x: 228
        y: 180
        text: qsTr("Quitter")
    }

    Connections {
        target: addLanguageButton
        onClicked: langModel.appendRow()
    }

    Button {
        id: addLanguageButton1
        x: 125
        y: 180
        text: qsTr("Supprimer")
    }

    Button {
        id: addLanguageButton2
        x: 11
        y: 180
        text: qsTr("Ajouter")
    }
}

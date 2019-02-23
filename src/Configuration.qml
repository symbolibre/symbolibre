import QtQuick 2.4

ConfigurationForm {


    // Different components use as delegates to edit the conf
    Component {
        id: delegateName
        TextInput {
            text: model.languageName
            onAccepted: {
                model.languageName = text
            }
        }
    }

    Component {
        id: delegateExtension
        TextInput {
            text: model.languageExtension
            onAccepted: {
                model.languageExtension = text
            }
        }
    }

    Component {
        id: delegateCmd
        TextInput {
            id: cmdText
            text: model.languageCmd
            onAccepted: {
                model.languageCmd = text
            }
        }

    }

}

import QtQuick 2.0
import QtQuick.Controls 2.3

Row {
    focus: true
    property var props

    id: row
    width: 320
    height: 32
    spacing: 10

    ParamText {}

    TextInput {
        property int val : 10
        id: textinput
        width: 140
        height: 32
        text: val
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 13
    }


    Keys.onLeftPressed: {
        if(textinput.val > 1) {
            textinput.val--
            configrw.write(props.key, textinput.val)
        }
    }

    Keys.onRightPressed: {
        textinput.val++
        configrw.write(props.key, textinput.val)
    }

    function init() {
        textinput.val = configrw.read(props.key)
    }
}






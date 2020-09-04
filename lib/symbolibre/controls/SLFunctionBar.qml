import QtQuick 2.12
import QtQuick.Controls 2.5

Row {
    height: 26

    // Make sure an empty bar model is always set, instead of "undefined"
    property var barmodel: undefined
    property var defmodel: SLFunctionBarModel {}

    // When a function key is clicked or touched
    signal activated(var action)

    SLFunctionKey {
        model: (barmodel || defmodel).f1
    }
    SLFunctionKey {
        model: (barmodel || defmodel).f2
    }
    SLFunctionKey {
        model: (barmodel || defmodel).f3
    }
    SLFunctionKey {
        model: (barmodel || defmodel).f4
    }
    SLFunctionKey {
        model: (barmodel || defmodel).f5
    }
}

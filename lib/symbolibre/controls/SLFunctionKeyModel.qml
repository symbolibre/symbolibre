import QtQuick 2.12

QtObject {
    // Function name (shown on the corresponding tab)
    property var text
    // Tab's background color (TODO: use an enumerated role)
    property var color
    // Signal when activated by F* key or click
    signal activated()
}

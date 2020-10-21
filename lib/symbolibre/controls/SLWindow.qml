// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    property Dialog errorDialog;

    Dialog {
        id: errorDialog
        anchors.centerIn: Overlay.overlay
        focus: true
        modal: true
        contentItem: Text {
            id: errorText
        }
        standardButtons: Dialog.Ok
    }

    function showError(text, title) {
        errorDialog.title = title || qsTr("Error");
        errorText.text = text;
        errorDialog.open();
    }
}

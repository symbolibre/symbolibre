// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.12

StackLayout {
    // When the layout gets focus, pass it to the active child
    onActiveFocusChanged: {
        children[currentIndex].forceActiveFocus()
    }
}

// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12

QtObject {
    default property list<QtObject> data

    // Get menu by name
    function get(name) {
        for (var i = 0; i < data.length; i++) {
            if (data[i].name == name)
                return data[i].model;
        }
    }
}

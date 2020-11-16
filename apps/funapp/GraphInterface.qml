// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.graph 0.2
import org.symbolibre.keyboard 0.2

GraphView {
    id: plotItem

    property ListModel functions

    mathContext: math
    cursorAttached: true

    // Send a signal saying that range changed. Send when lose focus
    function plot() {
        plotItem.clearGraph();
        plotItem.clearErrors();

        var i = 0
        for(i=0; i < functions.count; i++) {
            const f = functions.get(i);
            if (f.expr && f.active) {
                // Eliminate Edition Trees that reduce to empty expressions
                const formula = math.toGiac(f.expr);
                if (formula == "")
                    continue;

                const err = math.evalString(f.name + "(x):=" + formula);
                if (err == "") {
                    plotItem.addGraph(f.name, f.color);
                }
                else {
                    plotItem.addError(f.name + ": " + err.trim());
                }
            }
        }
    }

    // Show errors in a popup (if any)
    function showErrors() {
        if (plotItem.errors.length > 0) {
            errorMessages.text = plotItem.errors.join("\n");
            errorPopup.open();
        }
    }

    Keys.onPressed: {
        event.accepted = true;

        if (event.key === Qt.Key_Right) {
            plotItem.moveCursor(1, 0);

        } else if (event.key === Qt.Key_Left) {
            plotItem.moveCursor(-1, 0);

        } else if (event.key === Qt.Key_Up) {
            plotItem.moveCursor(0, 1);

        } else if (event.key === Qt.Key_Down) {
            plotItem.moveCursor(0, -1);

        } else if (event.key === Qt.Key_Plus) {
            plotItem.zoomIn(2.);

        } else if (event.key === Qt.Key_Minus) {
            plotItem.zoomIn(.5);

        } else if (event.key === SLKey.F1) {
            plotItem.cursorAttached = !plotItem.cursorAttached;

        } else {
            event.accepted = false
        }
    }
}

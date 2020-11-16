// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5

import org.symbolibre.applet 0.2

Row {
    height: 26
    width: 320

    // Make sure an empty bar model is always set, instead of "undefined"
    property var barModel: undefined
    property FunctionKeyModel defaultModel: FunctionKeyModel {
        text: ""
        color: palette.mid
    }

    // When a function key is clicked or touched
    signal activated(var action)

    SLFunctionKey {
        model: (barModel && barModel.f1) ? barModel.f1 : defaultModel
    }
    SLFunctionKey {
        model: (barModel && barModel.f2) ? barModel.f2 : defaultModel
    }
    SLFunctionKey {
        model: (barModel && barModel.f3) ? barModel.f3 : defaultModel
    }
    SLFunctionKey {
        model: (barModel && barModel.f4) ? barModel.f4 : defaultModel
    }
    SLFunctionKey {
        model: (barModel && barModel.f5) ? barModel.f5 : defaultModel
    }
}

import QtQuick 2.8
import QtQuick.Controls 2.2

Item {
    width: 320
    height: 240
    property alias buttonSND: buttonSND
    property alias buttonALPHA: buttonALPHA
    property alias buttonANS: buttonANS
    property alias buttonEXE: buttonEXE
    property alias buttonDEL: buttonDEL
    property alias buttonCLEAR: buttonCLEAR
    property alias buttonZ: buttonZ
    property alias buttonY: buttonY
    property alias buttonX: buttonX
    property alias buttonW: buttonW
    property alias buttonV: buttonV
    property alias buttonU: buttonU
    property alias buttonT: buttonT
    property alias buttonS: buttonS
    property alias buttonR: buttonR
    property alias buttonQ: buttonQ
    property alias buttonP: buttonP
    property alias buttonO: buttonO
    property alias buttonN: buttonN
    property alias buttonM: buttonM
    property alias buttonL: buttonL
    property alias buttonK: buttonK
    property alias buttonJ: buttonJ
    property alias buttonI: buttonI
    property alias buttonH: buttonH
    property alias buttonG: buttonG
    property alias buttonF: buttonF
    property alias buttonE: buttonE
    property alias buttonD: buttonD
    property alias buttonC: buttonC
    property alias buttonB: buttonB
    property alias buttonA: buttonA
    property alias buttonDown: buttonDown
    property alias buttonLeft: buttonLeft
    property alias buttonRight: buttonRight
    property alias buttonUp: buttonUp

    SpecialButton {
        id: buttonUp
        x: 268
        y: 0
        width: 30
        height: 20
        mainTextText: "↑"
    }

    SpecialButton {
        id: buttonRight
        x: 285
        y: 25
        width: 30
        height: 20
        mainTextText: "\u2192"
    }

    SpecialButton {
        id: buttonLeft
        x: 250
        y: 25
        width: 30
        height: 20
        mainTextText: "\u2190"
    }

    SpecialButton {
        id: buttonDown
        x: 268
        y: 50
        width: 30
        height: 20
        mainTextText: "\u2193"
    }

    Column {
        id: mainColumn
        spacing: 0

        property real h: 30
        property real w: 30
        property real hspacing: 10
        property real vspacing: 10

        property color bcolor: "#56d94f"
        property color pressedbcolor: "#238d1b"
        property color scolor: "#ffff00"
        property color pressedscolor: "#dbdb12"

        Column {
            id: topsection
            width: 250
            height: 80

            spacing: mainColumn.hspacing

            Row {
                id: arow
                leftPadding: 10
                rightPadding: 10
                spacing: mainColumn.vspacing

                SpecialButton {
                    id: buttonSND
                    mainTextText: "2nd"
                }

                CalcButton {
                    id: buttonA
                    alphaTextText: "A"
                    mainTextText: ""
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonB
                    alphaTextText: "B"
                    mainTextText: ""
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonC
                    alphaTextText: "C"
                    mainTextText: ""
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonD
                    alphaTextText: "D"
                    mainTextText: ""
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonE
                    alphaTextText: "E"
                    mainTextText: ""
                    sndTextText: ""
                }
            }

            Row {
                id: brow
                leftPadding: 10
                rightPadding: 10
                spacing: mainColumn.vspacing

                SpecialButton {
                    id: buttonALPHA
                    mainTextText: "Alpha"
                }

                CalcButton {
                    id: buttonF
                    alphaTextText: "F"
                    mainTextText: ""
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonG
                    alphaTextText: "G"
                    mainTextText: "("
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonH
                    alphaTextText: "H"
                    mainTextText: ")"
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonI
                    alphaTextText: "I"
                    mainTextText: "√"
                    sndTextText: ""
                }

                CalcButton {
                    id: buttonJ
                    alphaTextText: "J"
                    mainTextText: ":="
                    sndTextText: ""
                }
            }
        }

        Column {
            id: botsection
            width: 320
            height: 120

            spacing: mainColumn.hspacing

            Row {
                id: crow
                leftPadding: 65
                rightPadding: 65
                spacing: mainColumn.vspacing

                CalcButton {
                    id: buttonK
                    alphaTextText: "K"
                    mainTextText: "7"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonL
                    alphaTextText: "L"
                    mainTextText: "8"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonM
                    alphaTextText: "M"
                    mainTextText: "9"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                SpecialButton {
                    id: buttonDEL
                    mainTextText: "⌫"
                }

                SpecialButton {
                    id: buttonCLEAR
                    mainTextText: "Clear"
                }
            }

            Row {
                id: drow
                leftPadding: 65
                rightPadding: 65
                spacing: mainColumn.vspacing

                CalcButton {
                    id: buttonN
                    alphaTextText: "N"
                    mainTextText: "4"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonO
                    alphaTextText: "O"
                    mainTextText: "5"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonP
                    alphaTextText: "P"
                    mainTextText: "6"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonQ
                    alphaTextText: "Q"
                    mainTextText: "*"
                    sndTextText: "^"
                }

                CalcButton {
                    id: buttonR
                    alphaTextText: "R"
                    mainTextText: "/"
                    sndTextText: ""
                }
            }

            Row {
                id: erow
                leftPadding: 65
                rightPadding: 65
                spacing: mainColumn.vspacing

                CalcButton {
                    id: buttonS
                    alphaTextText: "S"
                    mainTextText: "1"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonT
                    alphaTextText: "T"
                    mainTextText: "2"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonU
                    alphaTextText: "U"
                    mainTextText: "3"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonV
                    alphaTextText: "V"
                    mainTextText: "+"
                    sndTextText: "∑"
                }

                CalcButton {
                    id: buttonW
                    alphaTextText: "W"
                    mainTextText: "-"
                    sndTextText: ""
                }
            }

            Row {
                id: frow
                leftPadding: 65
                rightPadding: 65
                spacing: mainColumn.vspacing

                CalcButton {
                    id: buttonX
                    alphaTextText: "X"
                    mainTextText: ""
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonY
                    alphaTextText: "Y"
                    mainTextText: "0"
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                CalcButton {
                    id: buttonZ
                    alphaTextText: "Z"
                    mainTextText: ","
                    sndTextText: ""
                    standardColor: "#cccccc"
                    pressedColor: "#888888"
                }

                SpecialButton {
                    id: buttonANS
                    mainTextText: "ANS"
                }

                SpecialButton {
                    id: buttonEXE
                    mainTextText: "EXE"
                }
            }
        }
    }
}

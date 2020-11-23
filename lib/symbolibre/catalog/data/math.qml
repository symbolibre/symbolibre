import QtQuick 2.12
import org.symbolibre.catalog 0.2

CatalogModel {
    CatalogPage {
        name: "root"
        title: qsTr("Math")

        ListModel {
            ListElement {
                title: qsTr("Base")
                jump: "basic"
            }
            ListElement {
                title: qsTr("Trigonometry")
                jump: "trig"
            }
            ListElement {
                title: qsTr("Arithmetic")
                jump: "arith"
            }
        }
    }

    CatalogPage {
        name: "basic"
        title: qsTr("Base")

        ListModel {
            ListElement {
                title: qsTr("Square root")
                doc: "sqrt"
                insert: '{"type": "sqrt", "content":[[""]]}'
            }
            ListElement {
                title: qsTr("Sum")
                doc: "sum"
                insert: '{"type": "sum", "content":[[""],[""]]}'
            }
            ListElement {
                title: qsTr("Integral")
                doc: "defint"
                insert: '{"type": "defint", "content":[[""],[""],[""]]}'
            }
            ListElement {
                title: qsTr("Antiderivative")
                doc: "int"
                insert: '{"type": "int", "content":[[""],[""]]}'
            }
            ListElement {
                title: qsTr("Absolute value")
                doc: "abs"
                insert: '["abs"]'
            }
            ListElement {
                title: qsTr("Int. part")
                doc: "iPart"
                insert: '["iPart"]'
            }
            ListElement {
                title: qsTr("Frac. part")
                doc: "frac"
                insert: '["frac"]'
            }
            ListElement {
                title: qsTr("Rounded value")
                doc: "round"
                insert: '["round"]'
            }
        }
    }

    CatalogPage {
        name: "trig"
        title: qsTr("Trigonometry")

        ListModel {
            ListElement {
                title: "π"
                doc: "pi"
                insert: '["π"]'
            }
            ListElement {
                title: "sin"
                insert: '["sin"]'
            }
            ListElement {
                title: "cos"
                insert: '["cos"]'
            }
            ListElement {
                title: "tan"
                insert: '["tan"]'
            }
            ListElement {
                title: "asin"
                insert: '["asin"]'
            }
            ListElement {
                title: "acos"
                insert: '["acos"]'
            }
            ListElement {
                title: "atan"
                insert: '["atan"]'
            }
        }
    }

    CatalogPage {
        name: "arith"
        title: qsTr("Arithmetic")

        ListModel {
            ListElement {
                title: qsTr("GCD")
                doc: "gcd"
                insert: '["gcd"]'
            }
            ListElement {
                title: qsTr("LCM")
                doc: "lcm"
                insert: '["lcm"]'
            }
            ListElement {
                title: qsTr("Quotient")
                doc: "iquo"
                insert: '["iquo"]'
            }
            ListElement {
                title: qsTr("Remainder")
                doc: "irem"
                insert: '["irem"]'
            }
        }
    }
}

import QtQuick 2.12
import org.symbolibre.catalog 0.2

CatalogModel {
    CatalogPage {
        name: "root"
        title: qsTr("Math")

        ListModel {
            ListElement {
                title: qsTr("Basic")
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
        title: qsTr("Basic")

        ListModel {
            ListElement {
                title: qsTr("sqrt")
                insert: '{"type": "sqrt", "content":[[""]]}'
            }
            ListElement {
                title: qsTr("sum")
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
                title: qsTr("abs")
                insert: '["abs"]'
            }
            ListElement {
                title: qsTr("iPart")
                insert: '["iPart"]'
            }
            ListElement {
                title: qsTr("frac")
                insert: '["frac"]'
            }
            ListElement {
                title: qsTr("round")
                insert: '["round"]'
            }
        }
    }

    CatalogPage {
        name: "trig"
        title: qsTr("Trigonometry")

        ListModel {
            ListElement {
                title: qsTr("π")
                doc: "pi"
                insert: '["π"]'
            }
            ListElement {
                title: qsTr("sin")
                insert: '["sin"]'
            }
            ListElement {
                title: qsTr("cos")
                insert: '["cos"]'
            }
            ListElement {
                title: qsTr("tan")
                insert: '["tan"]'
            }
            ListElement {
                title: qsTr("asin")
                insert: '["asin"]'
            }
            ListElement {
                title: qsTr("acos")
                insert: '["acos"]'
            }
            ListElement {
                title: qsTr("atan")
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

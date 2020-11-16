import QtQuick 2.12
import org.symbolibre.catalog 0.2

CatalogModel {
    CatalogPage {
        name: "root"
        title: qsTr("OCaml")

        ListModel {
            ListElement {
                title: "while"
                insert: "while * do\n\ndone;\n"
            }
            ListElement {
                title: "for"
                insert: "for * do\n\ndone;\n"
            }
            ListElement {
                title: "input"
                insert: "read_int(*)"
            }
            ListElement {
                title: "print"
                insert: "print_string(*)"
            }
            ListElement {
                title: "match"
                insert: "match * with\n |.. -> ..\n"
            }
            ListElement {
                title: "if"
                insert: "else\n"
            }
            ListElement {
                title: "let"
                insert: "let * = .. in"
            }
            ListElement {
                title: "let rec"
                insert: "let rec * = .. in"
            }
        }
    }
}

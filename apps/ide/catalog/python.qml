import QtQuick 2.12
import org.symbolibre.catalog 0.2

CatalogModel {
    CatalogPage {
        name: "root"
        title: qsTr("Python")

        ListModel {
            ListElement {
                title: qsTr("Import")
                jump: "import"
            }
            ListElement {
                title: qsTr("Conds, loops")
                jump: "flow"
            }
            ListElement {
                title: qsTr("Input/Output")
                jump: "io"
            }
            ListElement {
                title: qsTr("Computation")
                jump: "math"
            }
            ListElement {
                title: qsTr("Booleans")
                jump: "bool"
            }
            ListElement {
                title: qsTr("Conversion")
                jump: "conv"
            }
            ListElement {
                title: qsTr("Exceptions")
                jump: "except"
            }
        }
    }

    CatalogPage {
        name: "import"
        title: "Import"

        ListModel {
            ListElement {
                title: "import"
                insert: "import *"
            }
            ListElement {
                title: "from import"
                doc: "from-import"
                insert: "from * import"
            }
            ListElement {
                title: "from import *"
                doc: "from-import-star"
                insert: "from * import \\*"
            }
            ListElement {
                title: "math"
                doc: "from-math-import-star"
                insert: "from math import \\*"
            }
        }
    }

    CatalogPage {
        name: "flow"
        title: "Conds, loops"

        ListModel {
            ListElement {
                title: "if"
                insert: "if *:\n"
            }
            ListElement {
                title: "elif"
                doc: "if"
                insert: "elif *:\n"
            }
            ListElement {
                title: "else"
                doc: "if"
                insert: "else:\n"
            }
            ListElement {
                title: "for"
                insert: "for * in :\n"
            }
            ListElement {
                title: "for range"
                doc: "for"
                insert: "for * in range():\n"
            }
            ListElement {
                title: "while"
                insert: "while"
            }
            ListElement {
                title: "def"
                insert: "def"
            }
            ListElement {
                title: "break"
                insert: "break"
            }
            ListElement {
                title: "continue"
                insert: "continue"
            }
            ListElement {
                title: "return"
                insert: "return"
            }
        }
    }

    CatalogPage {
        name: "io"
        title: "Input/Output"

        ListModel {
            ListElement {
                title: "input"
                insert: "input(*)"
            }
            ListElement {
                title: "print"
                insert: "print(*)"
            }
        }
    }

    CatalogPage {
        name: "math"
        title: "Computation"

        ListModel {
            ListElement {
                title: "abs()"
                doc: "abs"
                insert: "abs(*)"
            }
            ListElement {
                title: "min()"
                doc: "min"
                insert: "min(*)"
            }
            ListElement {
                title: "max()"
                doc: "max"
                insert: "max(*)"
            }
            ListElement {
                title: "floor()"
                doc: "floor"
                insert: "floor(*)"
            }
            ListElement {
                title: "ceil()"
                doc: "ceil"
                insert: "ceil(*)"
            }
            ListElement {
                title: "round()"
                doc: "round"
                insert: "round(*)"
            }
            ListElement {
                title: "sum()"
                doc: "sum"
                insert: "sum(*)"
            }
            ListElement {
                title: "sorted()"
                doc: "sorted"
                insert: "sorted(*)"
            }
            ListElement {
                title: "zip()"
                doc: "zip"
                insert: "zip(*)"
            }
        }
    }

    CatalogPage {
        name: "bool"
        title: "Booleans"

        ListModel {
            ListElement {
                title: "True"
                insert: "True"
            }
            ListElement {
                title: "False"
                insert: "False"
            }
            ListElement {
                title: "not"
                insert: "not"
            }
            ListElement {
                title: "and"
                insert: "and"
            }
            ListElement {
                title: "or"
                insert: "or"
            }
            ListElement {
                title: "all()"
                doc: "all"
                insert: "all(*)"
            }
            ListElement {
                title: "any()"
                doc: "any"
                insert: "any(*)"
            }
        }
    }

    CatalogPage {
        name: "conv"
        title: "Conversion"

        ListModel {
            ListElement {
                title: "int()"
                insert: "int(*)"
            }
            ListElement {
                title: "float()"
                insert: "float(*)"
            }
            ListElement {
                title: "bool()"
                insert: "bool(*)"
            }
            ListElement {
                title: "str()"
                insert: "str(*)"
            }
            ListElement {
                title: "complex()"
                insert: "complex(*)"
            }
            ListElement {
                title: "list()"
                insert: "list(*)"
            }
            ListElement {
                title: "tuple()"
                insert: "tuple(*)"
            }
            ListElement {
                title: "set()"
                insert: "set(*)"
            }
            ListElement {
                title: "dict()"
                insert: "dict(*)"
            }
        }
    }

    CatalogPage {
        name: "except"
        title: "Exceptions"

        ListModel {
            ListElement {
                title: "try"
                insert: "try:\n*"
            }
            ListElement {
                title: "except"
                insert: "except *:\n"
            }
            ListElement {
                title: "except as"
                insert: "except * as :\n"
            }
            ListElement {
                title: "finally"
                insert: "finally:\n"
            }
            ListElement {
                title: "raise"
                insert: "raise *"
            }
        }
    }
}

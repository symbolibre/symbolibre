set(expr_SOURCES
    plugin.cpp
    SLQuickExpr.cpp
)
set(expr_QML_SOURCES
    ExprField.qml
    editiontree.js
)

add_library(exprplugin SHARED ${expr_SOURCES})
target_link_libraries(exprplugin Symbolibre::Symbolibre)

install(
    TARGETS exprplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/expr
)
install(
    FILES qmldir ${expr_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/expr
)

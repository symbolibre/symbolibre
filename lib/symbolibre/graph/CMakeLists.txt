set(graph_SOURCES
    customplotitem.cpp
    plugin.cpp
)
set(graph_HEADERS
    customplotitem.h
)

add_library(graphplugin SHARED ${graph_SOURCES})
target_link_libraries(graphplugin
    Giac::Giac
    QCustomPlot::QCustomPlot
    Qt5::Qml
    Qt5::Quick
    Qt5::Widgets
)
install(TARGETS graphplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)
install(FILES qmldir DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)
#install(FILES ${graph_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/graph)

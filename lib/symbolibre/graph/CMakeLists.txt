set(graph_SOURCES
    SLQuickGraph.cpp
)
set(graph_HEADERS
    SLQuickGraph.hpp
)
set(graphplugin_SOURCES
    plugin.cpp
)

add_library(graph OBJECT ${graph_SOURCES})
target_link_libraries(graph
    Giac
    QCustomPlot
    ${QT_DEPS}
    Qt5::Widgets
)

add_library(graphplugin SHARED ${graphplugin_SOURCES})
target_link_libraries(graphplugin
    Symbolibre::Symbolibre
)
install(TARGETS graphplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)
install(FILES ${graph_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/graph)
install(FILES qmldir DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)

set(graph_SOURCES
    SLQuickGraph.cpp
)
set(graph_HEADERS
    SLQuickGraph.hpp
)
set(graphplugin_SOURCES
    plugin.cpp
)

set(CMAKE_AUTOMOC OFF)
qt_wrap_cpp(graph_SOURCES ${graph_HEADERS})

add_library(graph OBJECT ${graph_SOURCES})
target_link_libraries(graph
    Giac
    QCustomPlot
    ${QT_DEPS}
    Qt::Widgets
)

set(CMAKE_AUTOMOC ON)

add_library(graphplugin SHARED ${graphplugin_SOURCES})
target_link_libraries(graphplugin
    Symbolibre::Symbolibre
    QCustomPlot
)
install(TARGETS graphplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)
install(FILES ${graph_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/graph)
install(FILES qmldir DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/graph)

set(graph_SOURCES
    curveitem.cpp
    customplotitem.cpp
)
set(graph_HEADERS
    curveitem.h
    customplotitem.h
)

add_library(graph OBJECT ${graph_SOURCES})
target_link_libraries(graph
    Giac::Giac
    QCustomPlot::QCustomPlot
    Qt5::Qml
    Qt5::Quick
    Qt5::Widgets
)
install(FILES ${graph_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/graph)

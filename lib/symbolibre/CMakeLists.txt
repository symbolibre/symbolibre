add_subdirectory(catalog)
add_subdirectory(controls)
add_subdirectory(editiontree)
add_subdirectory(expr)
add_subdirectory(graph)
add_subdirectory(keyboard)

configure_file(config.hpp.in config.hpp)
configure_file(config.qml.in config.qml)

set(slmath_SOURCES
    FileSystemContext.cpp
    MathContext.cpp
)
set(slmath_HEADERS
    editiontree.hpp
    FileSystemContext.hpp
    MathContext.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
)
set(slmath_QML_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/config.qml
)

add_library(${slmath_LIBRARY} SHARED ${slmath_SOURCES}
)
target_link_libraries(${slmath_LIBRARY} PUBLIC
    keyboard
    editiontree
    Giac::Giac
    ${QT_DEPS}
)

install(TARGETS ${slmath_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${slmath_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR})
install(FILES qmldir ${slmath_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/config)

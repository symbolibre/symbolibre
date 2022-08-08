add_subdirectory(applet)
add_subdirectory(cas)
add_subdirectory(catalog)
add_subdirectory(controls)
add_subdirectory(editiontree)
add_subdirectory(expr)
add_subdirectory(graph)
add_subdirectory(keyboard)
add_subdirectory(style)
add_subdirectory(util)

configure_file(config.hpp.in config.hpp)

set(symbolibre_HEADERS
    editiontree.hpp
)

add_library(Symbolibre SHARED)
target_link_libraries(Symbolibre
    PRIVATE applet keyboard cas editiontree graph util
    PUBLIC Giac ${QT_DEPS} Qt::QuickControls2 Qt::Widgets
)
target_include_directories(Symbolibre INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(Symbolibre::Symbolibre ALIAS Symbolibre)

install(TARGETS Symbolibre DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT SymbolibreTargets)
install(EXPORT SymbolibreTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Symbolibre
    NAMESPACE Symbolibre::)
install(FILES ${symbolibre_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR})

set(cas_SOURCES
    MathContext.cpp
)
set(casplugin_SOURCES
    plugin.cpp
)
set(cas_HEADERS
    MathContext.hpp
)

set(CMAKE_AUTOMOC OFF)
qt_wrap_cpp(cas_SOURCES ${cas_HEADERS})

add_library(cas OBJECT ${cas_SOURCES})
target_link_libraries(cas Giac ${QT_DEPS})

set(CMAKE_AUTOMOC ON)

add_library(casplugin SHARED ${casplugin_SOURCES})
target_link_libraries(casplugin Symbolibre::Symbolibre)

install(FILES ${cas_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/cas)
install(TARGETS casplugin DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/cas)
install(FILES qmldir DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/cas)

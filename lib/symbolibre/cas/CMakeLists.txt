set(cas_SOURCES
    MathContext.cpp
)
set(casplugin_SOURCES
    plugin.cpp
)
set(cas_HEADERS
    MathContext.hpp
)

add_library(cas OBJECT ${cas_SOURCES})
target_link_libraries(cas Giac::Giac ${QT_DEPS})

add_library(casplugin SHARED ${casplugin_SOURCES})
target_link_libraries(casplugin Symbolibre::Symbolibre)

install(FILES ${cas_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/cas)
install(TARGETS casplugin DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/cas)
install(FILES qmldir DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/cas)

set(util_SOURCES
    FileSystemSingleton.cpp
)
set(util_HEADERS
    FileSystemSingleton.hpp
)
set(utilplugin_SOURCES
    plugin.cpp
)

add_library(util OBJECT ${util_SOURCES})
target_link_libraries(util PUBLIC ${QT_DEPS})

add_library(utilplugin SHARED ${utilplugin_SOURCES})
target_link_libraries(utilplugin PUBLIC ${QT_DEPS})

install(TARGETS utilplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)
install(FILES qmldir
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)

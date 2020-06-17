set(util_SOURCES
    FileSystemSingleton.cpp
    plugin.cpp
)
set(util_HEADERS
    FileSystemSingleton.hpp
)

add_library(utilplugin SHARED ${util_SOURCES})
target_link_libraries(utilplugin PUBLIC ${QT_DEPS})

install(TARGETS utilplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)
install(FILES qmldir
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)

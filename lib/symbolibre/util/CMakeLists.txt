set(util_SOURCES
    FileSystemSingleton.cpp
)
set(util_HEADERS
    FileSystemSingleton.hpp
)
set(utilplugin_SOURCES
    plugin.cpp
)

set(CMAKE_AUTOMOC OFF)
qt5_wrap_cpp(util_SOURCES ${util_HEADERS})

add_library(util OBJECT ${util_SOURCES})
target_link_libraries(util PUBLIC ${QT_DEPS})

set(CMAKE_AUTOMOC ON)

add_library(utilplugin SHARED ${utilplugin_SOURCES})
target_link_libraries(utilplugin PUBLIC ${QT_DEPS})

install(FILES ${util_HEADERS}
    DESTINATION ${SL_INCLUDE_INSTALL_DIR}/util)
install(TARGETS utilplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)
install(FILES qmldir
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/util)

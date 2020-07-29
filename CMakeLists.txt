cmake_minimum_required(VERSION 3.1)

project(Symbolibre
    VERSION 0.1
    DESCRIPTION "Calculator software"
    HOMEPAGE_URL "https://symbolibre.org"
)

include(CTest)
include(GNUInstallDirs)
include(FeatureSummary)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 5.12 REQUIRED Core Gui Qml Quick Widgets OPTIONAL_COMPONENTS Test)
find_package(QCustomPlot REQUIRED)
find_package(Giac REQUIRED)

if(BUILD_TESTING AND NOT(${Qt5Test_FOUND}))
    message(SEND_ERROR "Tests enabled but Qt5Test not found")
endif()
set(USE_CUSTOM_QML_PATH ON CACHE BOOL
    "Install QML modules in a Symbolibre-specific location")

set(SL_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/symbolibre)
set(SL_DATA_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre)
set(SL_THEME_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre/theme)

get_filename_component(SL_FULL_INCLUDE_INSTALL_DIR ${SL_INCLUDE_INSTALL_DIR}
    ABSOLUTE BASE_DIR ${CMAKE_INSTALL_PREFIX})
get_filename_component(SL_FULL_DATA_INSTALL_DIR ${SL_DATA_INSTALL_DIR}
    ABSOLUTE BASE_DIR ${CMAKE_INSTALL_PREFIX})
get_filename_component(SL_FULL_THEME_INSTALL_DIR ${SL_THEME_INSTALL_DIR}
    ABSOLUTE BASE_DIR ${CMAKE_INSTALL_PREFIX})

set(SYMBOLIBRE_THEME_DIR ${SL_FULL_THEME_INSTALL_DIR})

if(USE_CUSTOM_QML_PATH)
    set(SL_QML_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/symbolibre/qml)
    get_filename_component(SL_FULL_QML_INSTALL_DIR ${SL_QML_INSTALL_DIR}
        ABSOLUTE BASE_DIR ${CMAKE_INSTALL_PREFIX})

    # *only* used by IDEs
    set(QML_IMPORT_PATH ${SL_FULL_QML_INSTALL_DIR} CACHE STRING "QML search paths for IDEs")

else()
    if(QMAKE_QT_EXECUTABLE) # set by Qt Creator
        set(QMAKE_COMMAND ${QMAKE_QT_EXECUTABLE} CACHE STRING "qmake executable")
    else()
        set(QMAKE_COMMAND qmake CACHE STRING "qmake executable")
    endif()
    execute_process(COMMAND "${QMAKE_COMMAND}" -query QT_INSTALL_QML
        RESULT_VARIABLE QML_INSTALL_DIR_QMAKE_STATUS
        OUTPUT_VARIABLE QML_INSTALL_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT(QML_INSTALL_DIR_QMAKE_STATUS EQUAL 0) OR NOT(QML_INSTALL_DIR))
        message(FATAL_ERROR "Unable to query qmake")
    endif()

    set(SL_FULL_QML_INSTALL_DIR "${QML_INSTALL_DIR}")
    if(NOT(SL_FULL_QML_INSTALL_DIR MATCHES "^${CMAKE_INSTALL_PREFIX}"))
        message(WARNING "QML modules will be installed outside the install prefix.")
    endif()
    message(STATUS "QML modules will be installed to ${SL_FULL_QML_INSTALL_DIR}")
    # FIXME should be a relative path
    set(SL_QML_INSTALL_DIR ${SL_FULL_QML_INSTALL_DIR})
endif()

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})

set(QT_DEPS Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)

set_property(GLOBAL PROPERTY CXX_STANDARD 14)

if(NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

feature_summary(WHAT ALL)

include_directories(lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(theme)
add_subdirectory(lib)
add_subdirectory(main_menu)
add_subdirectory(apps)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(cmake)

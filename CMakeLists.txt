cmake_minimum_required(VERSION 3.1)

project(symbolibre-math
    VERSION 0.1
    DESCRIPTION "Calculator software"
    HOMEPAGE_URL "https://symbolibre.org"
)

include(CTest)
include(GNUInstallDirs)
include(FeatureSummary)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(slmath_LIBRARY symbolibre)
set(SL_DATA_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre)
set(SL_THEME_INSTALL_DIR ${CMAKE_INSTALL_FULL_DATADIR}/symbolibre/theme)
set(SYMBOLIBRE_THEME_DIR ${SL_THEME_INSTALL_DIR})
set(SL_QML_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/symbolibre/qml)
set(SL_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/symbolibre)

set(QT_DEPS Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})

set_property(GLOBAL PROPERTY CXX_STANDARD 14)

if(NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

find_package(Qt5 5.12 REQUIRED Core Gui Qml Quick OPTIONAL_COMPONENTS Test)
find_package(QCustomPlot REQUIRED)
find_package(Giac REQUIRED)

if(BUILD_TESTING AND NOT(${Qt5Test_FOUND}))
    message(SEND_ERROR "Tests enabled but Qt5Test not found")
endif()

feature_summary(WHAT ALL)

include_directories(lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(theme)
add_subdirectory(lib)
add_subdirectory(apps)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

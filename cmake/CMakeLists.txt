include(CMakePackageConfigHelpers)
configure_package_config_file(SymbolibreConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SymbolibreConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Symbolibre
    PATH_VARS
        CMAKE_INSTALL_INCLUDEDIR
        SL_QML_INSTALL_DIR
        SL_DATA_INSTALL_DIR
        SL_APPLETS_INSTALL_DIR
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SymbolibreConfigVersion.cmake
    COMPATIBILITY SameMinorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SymbolibreConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SymbolibreConfigVersion.cmake
    FindGiac.cmake
    FindQCustomPlot.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Symbolibre
)

add_subdirectory(home)

if(ENABLE_APPS)
    add_subdirectory(calcsheet)
    add_subdirectory(funapp)
    if(SL_QT_VERSION EQUAL 5)
        add_subdirectory(ide)
    endif()
endif()

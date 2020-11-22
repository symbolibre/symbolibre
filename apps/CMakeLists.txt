add_subdirectory(home)

if(ENABLE_APPS)
    add_subdirectory(calcsheet)
    add_subdirectory(funapp)
    add_subdirectory(ide)
endif()

## generate
string(TIMESTAMP CONFIG_DATE)
set(CONFIG_DATE ${CONFIG_DATE} CACHE INTERNAL "Date when build was made")
set(CONFIG_USER $ENV{USER} CACHE INTERNAL "Name who did the build")
configure_file("${PROJECT_SOURCE_DIR}/cmake/mt.sniper-setup.sh.in" "${PROJECT_BINARY_DIR}/mt.sniper-setup.sh" @ONLY)

## install
install(FILES "${PROJECT_BINARY_DIR}/mt.sniper-setup.sh" DESTINATION ${CMAKE_INSTALL_DATADIR}/mt.sniper RENAME setup.sh)

## for cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MtSNiPERConfigVersion.cmake
    VERSION ${MtSNiPER_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# - install time config and target files
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/MtSNiPERConfig.cmake.in
    "${PROJECT_BINARY_DIR}/MtSNiPERConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
    )

# - install and export
install(FILES
    #"${PROJECT_BINARY_DIR}/MtSNiPERConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/MtSNiPERConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/"
    )

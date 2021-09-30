## generate
configure_file("${PROJECT_SOURCE_DIR}/cmake/mt.sniper-setup.sh.in" "${PROJECT_BINARY_DIR}/mt.sniper-setup.sh" @ONLY)

## install
install(FILES "${PROJECT_BINARY_DIR}/mt.sniper-setup.sh" DESTINATION ${CMAKE_INSTALL_DATADIR}/mt.sniper RENAME setup.sh)
# build
file(GLOB sources CONFIGURE_DEPENDS src/*.cc)
add_library(MtIOExample SHARED ${sources})
target_include_directories(MtIOExample PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(MtIOExample PRIVATE SniperMuster)

# install
install(TARGETS MtIOExample DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES python/MtIOExample.py DESTINATION ${CMAKE_INSTALL_PYTHONDIR})
install(FILES share/run-MtIO.py DESTINATION ${CMAKE_INSTALL_DATADIR})
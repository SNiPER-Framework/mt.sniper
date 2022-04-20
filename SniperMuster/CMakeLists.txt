# build
file(GLOB sources CONFIGURE_DEPENDS src/*.cc)
add_library(SniperMuster SHARED ${sources})
target_include_directories(SniperMuster PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> ${SNIPER_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(SniperMuster PUBLIC SNiPER::SniperKernel TBB::tbb ${PYTHON_LIBRARY} ${BOOST_PYTHON_LIBRARY})

#install
install(TARGETS SniperMuster
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_PYTHONDIR}/SniperMuster)
install(DIRECTORY SniperMuster DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY python/SniperMuster DESTINATION ${CMAKE_INSTALL_PYTHONDIR})
install(FILES share/run-MusterTest.py
              share/run-MtAlgDepGraph.py
        DESTINATION ${CMAKE_INSTALL_DATADIR})
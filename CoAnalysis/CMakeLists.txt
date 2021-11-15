PKG(CoAnalysis
    DEPENDS
        offline::EvtNavigator
        offline::RootIOSvc
        SniperMuster
)
# build
#file(GLOB sources CONFIGURE_DEPENDS src/*.cc)
#add_library(CoAnalysis SHARED ${sources})
target_include_directories(CoAnalysis PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#target_link_libraries(CoAnalysis PRIVATE SniperMuster)

# install
install(TARGETS CoAnalysis DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY python/CoAnalysis DESTINATION ${CMAKE_INSTALL_PYTHONDIR})
install(FILES share/run.py DESTINATION ${CMAKE_INSTALL_DATADIR})
cmake_minimum_required(VERSION 3.12)
message(STATUS "cmake version ${CMAKE_VERSION}")

project(MtSNiPER VERSION 1.0 LANGUAGES CXX)

find_package(SNiPER 2.0 REQUIRED)
include($ENV{TBB_ROOT}/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT $ENV{TBB_ROOT} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=0)
find_package(TBB)

# Global Settings
include(CMakePrintHelpers)
include(GNUInstallDirs)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DLINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DDARWIN)
    set(CMAKE_SHARED_LIBRARY_SUFFIX .so)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_SKIP_BUILD_RPATH FALSE)  
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
if(WIN32)
    add_definitions(-DWIN32)
endif()

# Compiler and Linker Settings
set(CMAKE_CXX_STANDARD 11 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed -Wl,--no-undefined")
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
endif()


#python
set(PyPkgName Python3)  #Use Python3 by default
option(USE_PYTHON2 "Use Python2 instead of Python3" OFF)
if(USE_PYTHON2)
    set(PyPkgName Python2)
endif()
find_package(${PyPkgName} COMPONENTS Interpreter Development REQUIRED)
message(STATUS "python version ${${PyPkgName}_VERSION}")
set(PYTHON_VERSION "${${PyPkgName}_VERSION}")
set(PYTHON_INCLUDE_DIRS "${${PyPkgName}_INCLUDE_DIRS}")
set(PYTHON_LIBRARY "${${PyPkgName}_LIBRARIES}")
#boost.python
set(Boost_USE_MULTITHREADED OFF)
set(BoostPyVer "${${PyPkgName}_VERSION_MAJOR}${${PyPkgName}_VERSION_MINOR}")
find_package(Boost 1.67 COMPONENTS python${BoostPyVer} REQUIRED)
set(BOOST_PYTHON_LIBRARY "${Boost_PYTHON${BoostPyVer}_LIBRARY}")
unset(BoostPyVer)
#python install dir
if(NOT CMAKE_INSTALL_PYTHONDIR)
    set(CMAKE_INSTALL_PYTHONDIR "python")
endif()
#...
add_definitions(-DHAVE_PYTHON)
unset(PyPkgName)

add_subdirectory(SniperMuster)
add_subdirectory(MtIOExample)

#Generate and install configure files
include(cmake/MtSNiPERCreateConfig.cmake)

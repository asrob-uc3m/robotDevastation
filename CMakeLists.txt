# Copyright: ASROB 2012 (C) Robotics Society of the Universidad Carlos III de Madrid
#     (http://asrob.uc3m.es) RD Project
#
# Authors:
# <a href="https://plus.google.com/+davidestevezfernandez">David Estevez</a> (2014-present),
# <a href="http://roboticslab.uc3m.es/roboticslab/people/jg-victores">Juan G. Victores</a> (2012-present),
# <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a> (2012-2014).
#
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see $RD_ROOT/LICENSE

cmake_minimum_required(VERSION 2.8.11)

project(robotDevastation)

##### program options
option(ENABLE_robotDevastation "Choose if you want to compile robotDevastation" TRUE)
option(ENABLE_rdServer "Choose if you want to compile rdServer" TRUE)

##### tests options
option(ENABLE_tests "Choose if you want to compile tests" TRUE)

if(MSVC)
    message(STATUS "Running on windows")    
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
        "Choose the type of build, recommanded options are: Debug or Release")
endif()
# Hide variable to MSVC users, since it is not needed
if(MSVC)
    mark_as_advanced(CMAKE_BUILD_TYPE)
endif()

######################

### this makes everything go in $RD_DIR/lib and $RD_DIR/bin
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Libraries go to ${LIBRARY_OUTPUT_PATH}")
message(STATUS "Executables go to ${EXECUTABLE_OUTPUT_PATH}")

##########################################

# Pick up our scripts - they are all in the /cmake subdirectory
set(RD_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${RD_MODULE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage") # enabling coverage
endif()

# add main contents
add_subdirectory(share)
add_subdirectory(src)

if(ENABLE_tests)
    add_subdirectory(test)
endif()

# uninstall target
configure_file("${CMAKE_SOURCE_DIR}/cmake/template/RDConfigUninstall.cmake.in"
               "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/RDConfigUninstall.cmake)

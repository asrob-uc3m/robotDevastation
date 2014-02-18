# Copyright: ASROB 2012 (C) Robotics Society of the Universidad Carlos III de Madrid
#     (http://asrob.uc3m.es) RD Project
#
# Authors:
# <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>.
# <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a>,
#
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see $RD_ROOT/main/doc/LGPL.TXT

# Reduce warning level with cmake 2.6
cmake_minimum_required(VERSION 2.6)
# Cmake policies
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(robotDevastation)

##### library options
option(ENABLE_RdInputLib "Choose if you want to compile RdInputLib" TRUE)
option(ENABLE_RdManagerLib "Choose if you want to compile RdManagerLib" TRUE)
option(ENABLE_RdRobotLib "Choose if you want to compile RdRobotLib" TRUE)

##### program options
option(ENABLE_rdClient "Choose if you want to compile rdClient" TRUE)
option(ENABLE_rdSim "Choose if you want to compile rdSim" TRUE)


if(MSVC)
  MESSAGE(STATUS "Running on windows")    
  set(CMAKE_DEBUG_POSTFIX "d")
endif(MSVC)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
    "Choose the type of build, recommanded options are: Debug or Release")
endif(NOT CMAKE_BUILD_TYPE)
# Hide variable to MSVC users, since it is not needed
if (MSVC)
   mark_as_advanced(CMAKE_BUILD_TYPE)
endif(MSVC)

######################

### this makes everything go in $RD_DIR/lib and $RD_DIR/bin
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Libraries go to ${LIBRARY_OUTPUT_PATH}")
message(STATUS "Executables go to ${EXECUTABLE_OUTPUT_PATH}")
# this doesn't happen automatically for makefiles
make_directory(${LIBRARY_OUTPUT_PATH})
make_directory(${EXECUTABLE_OUTPUT_PATH})
# and let us clean their contents on a "make clean"
##set_directory_properties(PROPERTIES LIBRARY_OUTPUT_PATH ADDITIONAL_MAKE_CLEAN_FILES)
##set_directory_properties(PROPERTIES EXECUTABLE_OUTPUT_PATH ADDITIONAL_MAKE_CLEAN_FILES)
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
mark_as_advanced(CMAKE_BACKWARDS_COMPATIBILITY)
##########################################

# Pick up our scripts - they are all in the conf subdirectory
set(RD_MODULE_PATH ${CMAKE_SOURCE_DIR}/conf)

#set_property(GLOBAL PROPERTY RD_INCLUDE_DIRS)
#set_property(GLOBAL PROPERTY RD_LINK_DIRS)
#set_property(GLOBAL PROPERTY RD_LIBRARIES)
#set_property(GLOBAL PROPERTY RD_TARGETS)
set(RD_INCLUDE_DIRS CACHE INTERNAL "appended header dirs" FORCE)
set(RD_LINK_DIRS CACHE INTERNAL "appended link dirs" FORCE)
set(RD_LIBRARIES CACHE INTERNAL "appended libraries" FORCE)

# add main contents
add_subdirectory(src)

# export our variables to a RDConfig.cmake creation
set(RD_LINK_DIRS ${RD_LINK_DIRS} ${LIBRARY_OUTPUT_PATH})
configure_file(${CMAKE_SOURCE_DIR}/conf/template/RDConfig.cmake.in
               ${CMAKE_BINARY_DIR}/RDConfig.cmake @ONLY)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf/template/RDConfigUninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RDConfigUninstall.cmake" IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/RDConfigUninstall.cmake)

#MESSAGE("\nDon't forget to set: " ${CMAKE_BINARY_DIR} " as your \${RD_DIR} !!!\n")


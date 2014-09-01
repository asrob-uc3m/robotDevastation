# Copyright: Universidad Carlos III de Madrid (C) 2014;
# Authors: Juan G. Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

# reduce warning level with cmake 2.6
cmake_minimum_required(VERSION 2.6)
#cmake policies
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(rdRpi)

##### options
option(ENABLE_testRpiYarp "Choose if you want to compile testAsrobPlugins" FALSE)
option(ENABLE_testNanoBot "Choose if you want to compile testSkymegaBot" FALSE)

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

### this makes everything go in $RPI_DIR/lib and $RPI_DIR/bin
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
set(RPI_MODULE_PATH ${CMAKE_SOURCE_DIR}/conf)

#set_property(GLOBAL PROPERTY RPI_INCLUDE_DIRS)
#set_property(GLOBAL PROPERTY RPI_LINK_DIRS)
#set_property(GLOBAL PROPERTY RPI_LIBRARIES)
#set_property(GLOBAL PROPERTY RPI_TARGETS)
set(RPI_INCLUDE_DIRS CACHE INTERNAL "appended header dirs" FORCE)
set(RPI_LINK_DIRS CACHE INTERNAL "appended link dirs" FORCE)
set(RPI_LIBRARIES CACHE INTERNAL "appended libraries" FORCE)

# add main contents
add_subdirectory(src)

# export our variables to a RPIConfig.cmake creation
#j#set(RPI_LINK_DIRS ${RPI_LINK_DIRS} ${LIBRARY_OUTPUT_PATH})
#j#configure_file(${CMAKE_SOURCE_DIR}/conf/template/RPIConfig.cmake.in
#j#               ${CMAKE_BINARY_DIR}/RPIConfig.cmake @ONLY)

# create a symbolic link to the shared directory (as contains models and may be heavy)
# If you want a deep copy, you can use the copy_directory command instead of create_symlink.
#j#add_custom_target(install_applications
#j#    "${CMAKE_COMMAND}" -E "create_symlink" "${CMAKE_SOURCE_DIR}/app" "$ENV{RPI_ROOT}/share")

#endif(MSVC)
##add_custom_target(nuke
##  "${CMAKE_COMMAND}" -E "remove_directory" "${CMAKE_SOURCE_DIR}/bin" ";"
##  "${CMAKE_COMMAND}" -E "remove_directory" "${CMAKE_SOURCE_DIR}/lib")

#MESSAGE("\nDon't forget to set: " ${CMAKE_BINARY_DIR} " as your \${RPI_DIR} !!!\n")


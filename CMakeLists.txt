# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(robotDevastation LANGUAGES CXX)

if(WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

# Let the user specify a configuration (only single-config generators).
if(NOT CMAKE_CONFIGURATION_TYPES)
    # Possible values.
    set(_configurations Debug Release MinSizeRel RelWithDebInfo)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${_configurations})

    foreach(_conf ${_configurations})
        set(_conf_string "${_conf_string} ${_conf}")
    endforeach()

    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING
                 "Choose the type of build, options are:${_conf_string}")

    if(NOT CMAKE_BUILD_TYPE)
        # Encourage the user to specify build type.
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE Release)
    endif()
endif()

# Pick up our CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
                              ${CMAKE_SOURCE_DIR}/cmake/modules
                              ${CMAKE_SOURCE_DIR}/cmake/find-modules)

# Find project's hard dependencies.
find_package(YCM 0.10 REQUIRED)
find_package(COLOR_DEBUG REQUIRED)
find_package(ASROB_YARP_DEVICES REQUIRED)
find_package(YARP 3.0 REQUIRED COMPONENTS OS dev sig)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZBar REQUIRED)

# Find project's soft dependencies.
find_package(Doxygen QUIET)
find_package(GTestSources 1.6.0 QUIET)

# Standard installation directories.
include(GNUInstallDirs)

# Control where libraries and executables are placed during the build.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

option(ENABLE_coverage "Choose if you want to enable coverage" OFF)

if(ENABLE_coverage)
    # List supported compilers.
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

option(BUNDLE_DEPS "Choose if you want to bundle system and 3rd-party dependencies on install" OFF)
mark_as_advanced(BUNDLE_DEPS)

# Let CMake organize projects in folders (in IDEs that support this).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Create targets if specific requirements are satisfied.
include(CMakeDependentOption)

# Add main contents.
add_subdirectory(share)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(doc)

if(BUNDLE_DEPS AND WIN32)
    # Install system runtime libraries.
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT runtime)
    include(InstallRequiredSystemLibraries)
endif()

# packaging
include(RDPackage)

# Uninstall target.
include(AddUninstallTarget)

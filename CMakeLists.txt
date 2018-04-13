# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(robotDevastation LANGUAGES CXX)

if(MSVC)
    message(STATUS "Running on windows")
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
                              ${CMAKE_SOURCE_DIR}/cmake/find-modules
                              ${CMAKE_SOURCE_DIR}/cmake/ycm-modules)

# Bootstrap YCM. Make sure it's compatible with cmake_minimum_required().
set(YCM_TAG v0.6.0)
include(YCMBootstrap)

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

# Superbuild phase - find or download remote packages.
include(FindOrBuildPackage)
find_or_build_package(COLOR_DEBUG)
find_or_build_package(ASROB_YARP_DEVICES)

# find_or_build_package() doesn't bring into scope COLOR_DEBUG_INCLUDE_DIRS
# nor other config variables. If a system copy is found, we have to call the
# find_package() command and proceed as usual.
if(USE_SYSTEM_COLOR_DEBUG)
    find_package(COLOR_DEBUG REQUIRED)
    include_directories(${COLOR_DEBUG_INCLUDE_DIRS})
    add_library(COLOR_DEBUG UNKNOWN IMPORTED)
endif()

if(USE_SYSTEM_ASROB_YARP_DEVICES)
    find_package(ASROB_YARP_DEVICES REQUIRED)
    add_library(ASROB_YARP_DEVICES UNKNOWN IMPORTED)
endif()

# Prepare install/ directory (might be empty).
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/install)

# Install downloaded packages.
install(DIRECTORY ${CMAKE_BINARY_DIR}/install/
        DESTINATION ${CMAKE_INSTALL_PREFIX})

# let CMake organize projects in folders (in IDEs that support this)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Create targets if specific requirements are satisfied.
include(CMakeDependentOption)

# Find YARP (main dependency).
find_package(YARP REQUIRED)

# Load YARP modules.
if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
    include(YarpPlugin)
    include(YarpInstallationHelpers)
endif()

# Find SDL and ZBar.
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZBar REQUIRED)

# Find googletest sources (for unit testing).
find_package(GTestSources 1.6.0 QUIET)

# Configure installation paths for YARP resources.
yarp_configure_external_installation(rd)

# Add main contents.
add_subdirectory(share)
add_subdirectory(src)
add_subdirectory(tests)

# Generate documentation.
find_package(Doxygen QUIET)

cmake_dependent_option(GENERATE_doc_html "Choose if you want to generate HTML documentation through Doxygen" OFF
                       DOXYGEN_FOUND OFF)

if(GENERATE_doc_html)
    if(CMAKE_GENERATOR MATCHES "^(Visual Studio|Xcode)")
        set(_dox DOX)
    else()
        set(_dox dox)
    endif()

    add_custom_target(${_dox}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
                      COMMENT "Generating doxygen documentation"
                      SOURCES ${CMAKE_SOURCE_DIR}/doc/Doxyfile)

    set_property(TARGET ${_dox} PROPERTY FOLDER CMakePredefinedTargets)

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/html
            DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

# Uninstall target.
include(AddUninstallTarget)

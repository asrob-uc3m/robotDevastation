# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_robotDevastation "Choose if you want to compile robotDevastation" ON)

if(ENABLE_robotDevastation)

# Find YARP.  Point the YARP_DIR environment variable at your build.
find_package(YARP REQUIRED)

add_executable(robotDevastation main.cpp
                                RobotDevastation.hpp
                                RobotDevastation.cpp)

target_include_directories(robotDevastation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                   ${YARP_INCLUDE_DIRS})

target_link_libraries(robotDevastation YARP::YARP_OS
                                       UtilsLib
                                       MusicLib
                                       MentalMapLib
                                       InputLib
                                       NetworkLib
                                       RobotLib
                                       StateMachineLib
                                       ImageLib
                                       UserInterfaceLib
                                       GameStatesLib)

if(WIN32)
    target_compile_definitions(robotDevastation PUBLIC SDL_MAIN_HANDLED)
endif()

install(TARGETS robotDevastation
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT rd_game)

if(BUNDLE_DEPS)
    set(_bundled_app robotDevastation)

    configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/fixup_bundle.cmake.in
                   ${CMAKE_BINARY_DIR}/fixup_bundle_robotDevastation.cmake @ONLY)

    install(SCRIPT ${CMAKE_BINARY_DIR}/fixup_bundle_robotDevastation.cmake
            COMPONENT rd_game)
endif()

# Organize targets in folders (Visual Studio)
set_target_properties(robotDevastation PROPERTIES FOLDER Programs)

endif() # ENABLE_robotDevastation

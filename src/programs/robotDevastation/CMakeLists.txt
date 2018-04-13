# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_robotDevastation "Choose if you want to compile robotDevastation" ON)

if(ENABLE_robotDevastation)

add_executable(robotDevastation main.cpp
                                RobotDevastation.hpp
                                RobotDevastation.cpp)

add_dependencies(robotDevastation COLOR_DEBUG ASROB_YARP_DEVICES)

target_include_directories(robotDevastation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(robotDevastation PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(robotDevastation YARP::YARP_OS
                                       MusicLib
                                       MentalMapLib
                                       InputLib
                                       NetworkLib
                                       StateMachineLib
                                       ImageLib
                                       UserInterfaceLib
                                       GameStatesLib
                                       ASROB::RobotInterfaces)

if(WIN32)
    target_compile_definitions(robotDevastation PUBLIC SDL_MAIN_HANDLED)
endif()

install(TARGETS robotDevastation DESTINATION ${CMAKE_INSTALL_BINDIR})

# Organize targets in folders (Visual Studio)
set_target_properties(robotDevastation PROPERTIES FOLDER Programs)

endif() # ENABLE_robotDevastation

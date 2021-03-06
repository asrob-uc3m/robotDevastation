# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_robotDevastation "Choose if you want to compile robotDevastation" ON)

if(ENABLE_robotDevastation)

add_executable(robotDevastation main.cpp
                                RobotDevastation.hpp
                                RobotDevastation.cpp)

target_include_directories(robotDevastation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(robotDevastation YARP::YARP_os
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

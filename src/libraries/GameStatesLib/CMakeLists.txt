# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(GameStatesLib DeadState.hpp
                          DeadState.cpp
                          GameState.hpp
                          GameState.cpp
                          InitState.hpp
                          InitState.cpp)

add_dependencies(GameStatesLib COLOR_DEBUG ASROB_YARP_DEVICES)

target_include_directories(GameStatesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GameStatesLib PUBLIC StateMachineLib
                                           InputLib
                                           NetworkLib
                                           ImageLib
                                           MentalMapLib
                                           MusicLib
                                           UserInterfaceLib
                                           ASROB::RobotInterfaces)

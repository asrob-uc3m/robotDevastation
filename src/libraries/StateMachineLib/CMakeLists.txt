# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(StateMachineLib MockState.hpp
                            MockState.cpp
                            State.hpp
                            State.cpp
                            StateDirector.hpp
                            StateDirector.cpp
                            StateMachine.hpp
                            StateMachine.cpp
                            StateMachineBuilder.hpp
                            StateMachineBuilder.cpp
                            YarpStateDirector.hpp
                            YarpStateDirector.cpp)

target_include_directories(StateMachineLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(StateMachineLib PUBLIC YARP::YARP_OS
                                      PRIVATE ROBOTICSLAB::ColorDebug)

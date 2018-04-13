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

add_dependencies(StateMachineLib COLOR_DEBUG)

target_include_directories(StateMachineLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(StateMachineLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(StateMachineLib PUBLIC YARP::YARP_OS)

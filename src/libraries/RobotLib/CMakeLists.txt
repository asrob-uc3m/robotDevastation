# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(RobotLib MockRobotManager.cpp
                     MockRobotManager.hpp)

add_dependencies(RobotLib COLOR_DEBUG ASROB_YARP_DEVICES)

target_include_directories(RobotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(RobotLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(RobotLib PUBLIC ASROB::RobotInterfaces)

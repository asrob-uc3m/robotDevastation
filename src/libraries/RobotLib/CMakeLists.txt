# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(YARP REQUIRED)

add_library(RobotLib MockRobotManager.cpp
                     MockRobotManager.hpp)

target_include_directories(RobotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${YARP_INCLUDE_DIRS})

target_link_libraries(RobotLib PUBLIC ASROB::RobotInterfaces)

target_link_libraries(RobotLib PRIVATE ROBOTICSLAB::ColorDebug)

# Copyright: 2014 ASROB
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 

find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpInstallationHelpers)
yarp_configure_external_installation(rd)

### Go through directories

add_subdirectory(robotDevastation)
add_subdirectory(images)
add_subdirectory(sounds)
add_subdirectory(fonts)

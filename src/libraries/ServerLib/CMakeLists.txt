# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(YARP REQUIRED)

add_library(ServerLib RpcResponder.hpp
                      RpcResponder.cpp
                      Server.hpp
                      Server.cpp)

target_include_directories(ServerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                            ${YARP_INCLUDE_DIRS})

target_link_libraries(ServerLib PUBLIC YARP::YARP_OS
                                       MentalMapLib)

target_link_libraries(ServerLib PRIVATE UtilsLib)

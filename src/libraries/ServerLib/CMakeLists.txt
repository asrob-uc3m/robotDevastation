# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(ServerLib RpcResponder.hpp
                      RpcResponder.cpp
                      Server.hpp
                      Server.cpp)

add_dependencies(ServerLib COLOR_DEBUG)

target_include_directories(ServerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(ServerLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(ServerLib PUBLIC YARP::YARP_OS
                                       MentalMapLib)

target_link_libraries(ServerLib PRIVATE UtilsLib)

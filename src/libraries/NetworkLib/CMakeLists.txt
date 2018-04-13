# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(NetworkLib MockNetworkEventListener.hpp
                       MockNetworkEventListener.cpp
                       MockNetworkManager.hpp
                       MockNetworkManager.cpp
                       NetworkEventListener.hpp
                       NetworkManager.hpp
                       NetworkManager.cpp
                       YarpNetworkManager.hpp
                       YarpNetworkManager.cpp)

add_dependencies(NetworkLib COLOR_DEBUG)

target_include_directories(NetworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(NetworkLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()


target_link_libraries(NetworkLib PUBLIC YARP::YARP_OS
                                        MentalMapLib)

target_link_libraries(NetworkLib PRIVATE UtilsLib)

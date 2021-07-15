# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(UtilsLib ManagerHub.hpp
                     ManagerHub.cpp
                     SDLUtils.hpp
                     SDLUtils.cpp
                     Vector2dBase.hpp
                     Vocabs.hpp)

target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${SDL2_INCLUDE_DIR})

target_include_directories(UtilsLib PRIVATE ${SDL2_IMAGE_INCLUDE_DIR}
                                            ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(UtilsLib PUBLIC YARP::YARP_os
                                      ${SDL2_LIBRARY}
                                      ImageLib
                                      InputLib
                                      MentalMapLib
                                      NetworkLib
                                      MusicLib
                                      UserInterfaceLib
                                      ASROB::RobotInterfaces
                               PRIVATE ${SDL2_IMAGE_LIBRARIES}
                                       ${SDL2_TTF_LIBRARIES})

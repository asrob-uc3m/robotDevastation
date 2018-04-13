# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(UtilsLib Hub.hpp
                     Hub.cpp
                     SDLUtils.hpp
                     SDLUtils.cpp
                     Vector2dBase.hpp
                     Vocabs.hpp)

add_dependencies(UtilsLib COLOR_DEBUG ASROB_YARP_DEVICES)

target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${SDL2_INCLUDE_DIR})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(UtilsLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_include_directories(UtilsLib PRIVATE ${SDL2_IMAGE_INCLUDE_DIR}
                                            ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(UtilsLib PUBLIC YARP::YARP_OS
                                      ${SDL2_LIBRARY}
                                      ImageLib
                                      InputLib
                                      MentalMapLib
                                      NetworkLib
                                      MusicLib
                                      UserInterfaceLib
                                      ASROB::RobotInterfaces)

target_link_libraries(UtilsLib PRIVATE ${SDL2_IMAGE_LIBRARIES}
                                       ${SDL2_TTF_LIBRARIES})

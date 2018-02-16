# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(YARP REQUIRED)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(UtilsLib Hub.hpp
                     Hub.cpp
                     IniReader.hpp
                     SDLUtils.hpp
                     SDLUtils.cpp
                     Utils.hpp
                     Vector2dBase.hpp
                     Vocabs.hpp)

add_dependencies(UtilsLib COLOR_DEBUG ASROB_YARP_DEVICES)

target_include_directories(UtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${YARP_INCLUDE_DIRS}
                                           ${SDL2_INCLUDE_DIR})

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

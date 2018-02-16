# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(YARP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_library(MusicLib AudioManager.hpp
                     AudioManager.cpp
                     MockAudioManager.hpp
                     MockAudioManager.cpp
                     SDLAudioManager.hpp
                     SDLAudioManager.cpp)

add_dependencies(MusicLib COLOR_DEBUG)

target_include_directories(MusicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${YARP_INCLUDE_DIRS}
                                           ${SDL2_INCLUDE_DIR}
                                           ${SDL2_MIXER_INCLUDE_DIRS})

target_link_libraries(MusicLib PUBLIC YARP::YARP_OS
                                      ${SDL2_LIBRARY}
                                      ${SDL2_MIXER_LIBRARIES})

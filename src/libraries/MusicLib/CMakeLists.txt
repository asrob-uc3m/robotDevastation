# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

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
                                           ${SDL2_INCLUDE_DIR}
                                           ${SDL2_MIXER_INCLUDE_DIRS})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(MusicLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(MusicLib PUBLIC YARP::YARP_OS
                                      ${SDL2_LIBRARY}
                                      ${SDL2_MIXER_LIBRARIES})

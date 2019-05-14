# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(MusicLib AudioManager.hpp
                     AudioManager.cpp
                     MockAudioManager.hpp
                     MockAudioManager.cpp
                     SDLAudioManager.hpp
                     SDLAudioManager.cpp)

target_include_directories(MusicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${SDL2_INCLUDE_DIR}
                                           ${SDL2_MIXER_INCLUDE_DIRS})

target_link_libraries(MusicLib PUBLIC YARP::YARP_OS
                                      ${SDL2_LIBRARY}
                                      ${SDL2_MIXER_LIBRARIES}
                               PRIVATE ROBOTICSLAB::ColorDebug)

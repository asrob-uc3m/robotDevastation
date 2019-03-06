# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(MentalMapLib MentalMap.hpp
                         MentalMap.cpp
                         MentalMapEventListener.hpp
                         Player.hpp
                         Player.cpp
                         Target.hpp
                         Target.cpp
                         Weapon.hpp
                         Weapon.cpp)

target_include_directories(MentalMapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MentalMapLib PUBLIC NetworkLib
                                          MusicLib
                                          UtilsLib)

target_link_libraries(MentalMapLib PRIVATE ROBOTICSLAB::ColorDebug)

# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(UserInterfaceLib DeadScreen.hpp
                             DeadScreen.cpp
                             GameScreen.hpp
                             GameScreen.cpp
                             InitScreen.hpp
                             InitScreen.cpp
                             MockScreen.hpp
                             MockScreen.cpp
                             Screen.hpp
                             Screen.cpp
                             ScreenManager.hpp
                             ScreenManager.cpp
                             SDLScreenManager.hpp
                             SDLScreenManager.cpp
                             LogComponent.hpp
                             LogComponent.cpp)

target_include_directories(UserInterfaceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                   ${SDL2_INCLUDE_DIR}
                                                   ${SDL2_TTF_INCLUDE_DIRS})

target_include_directories(UserInterfaceLib PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(UserInterfaceLib PUBLIC YARP::YARP_os
                                              ${SDL2_LIBRARY}
                                              ${SDL2_TTF_LIBRARIES}
                                              ImageLib
                                              MentalMapLib
                                       PRIVATE ${SDL2_IMAGE_LIBRARIES}
                                               UtilsLib)

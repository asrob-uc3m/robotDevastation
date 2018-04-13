# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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
                             SDLScreenManager.cpp)

add_dependencies(UserInterfaceLib COLOR_DEBUG)

target_include_directories(UserInterfaceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                   ${SDL2_INCLUDE_DIR}
                                                   ${SDL2_TTF_INCLUDE_DIRS})

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(UserInterfaceLib PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_include_directories(UserInterfaceLib PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(UserInterfaceLib PUBLIC YARP::YARP_OS
                                              ${SDL2_LIBRARY}
                                              ${SDL2_TTF_LIBRARIES}
                                              ImageLib
                                              MentalMapLib)

target_link_libraries(UserInterfaceLib PRIVATE ${SDL2_IMAGE_LIBRARIES}
                                               UtilsLib)

# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

add_library(InputLib InputEventListener.hpp
                     InputManager.hpp
                     InputManager.cpp
                     Key.hpp
                     Key.cpp
                     MockInputEventListener.hpp
                     MockInputEventListener.cpp
                     MockInputManager.hpp
                     MockInputManager.cpp
                     SDLEventFactory.hpp
                     SDLEventFactory.cpp
                     SDLInputManager.hpp
                     SDLInputManager.cpp
                     WindowEvent.hpp
                     WindowEvent.cpp)

add_dependencies(InputLib COLOR_DEBUG)

target_include_directories(InputLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${SDL2_INCLUDE_DIR})

target_link_libraries(InputLib PUBLIC ${SDL2_LIBRARY})

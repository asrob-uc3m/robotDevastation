# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_rdServer "Choose if you want to compile rdServer" ON)

if(ENABLE_rdServer)

# Set up our main executable.
add_executable(rdServer main.cpp)

add_dependencies(rdServer COLOR_DEBUG)

if(YARP_VERSION_SHORT VERSION_LESS 2.3.70)
    target_include_directories(rdServer PUBLIC ${YARP_INCLUDE_DIRS})
endif()

target_link_libraries(rdServer YARP::YARP_init
                               YARP::YARP_OS
                               ServerLib)

if(WIN32)
    target_compile_definitions(rdServer PUBLIC SDL_MAIN_HANDLED)
endif()

install(TARGETS rdServer DESTINATION ${CMAKE_INSTALL_BINDIR})

# Organize targets in folders (Visual Studio)
set_target_properties(rdServer PROPERTIES FOLDER Programs)

endif() # ENABLE_rdServer

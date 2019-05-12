# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_rdServer "Choose if you want to compile rdServer" ON)

if(ENABLE_rdServer)

# Set up our main executable.
add_executable(rdServer main.cpp)

target_include_directories(rdServer PUBLIC ${YARP_INCLUDE_DIRS})

target_link_libraries(rdServer YARP::YARP_init
                               YARP::YARP_OS
                               ServerLib
                               ROBOTICSLAB::ColorDebug)

if(WIN32)
    target_compile_definitions(rdServer PUBLIC SDL_MAIN_HANDLED)
endif()

install(TARGETS rdServer
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT rd_server)

if(BUNDLE_DEPS AND WIN32)
    set(_bundled_app rdServer)

    configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/fixup_bundle.cmake.in
                   ${CMAKE_BINARY_DIR}/fixup_bundle_rdServer.cmake @ONLY)

    install(SCRIPT ${CMAKE_BINARY_DIR}/fixup_bundle_rdServer.cmake
            COMPONENT rd_server)
endif()

# Organize targets in folders (Visual Studio)
set_target_properties(rdServer PROPERTIES FOLDER Programs)

endif() # ENABLE_rdServer

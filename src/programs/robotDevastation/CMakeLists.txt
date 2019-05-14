# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

option(ENABLE_robotDevastation "Choose if you want to compile robotDevastation" ON)

if(ENABLE_robotDevastation)

add_executable(robotDevastation main.cpp
                                RobotDevastation.hpp
                                RobotDevastation.cpp)

target_include_directories(robotDevastation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(robotDevastation YARP::YARP_OS
                                       MusicLib
                                       MentalMapLib
                                       InputLib
                                       NetworkLib
                                       StateMachineLib
                                       ImageLib
                                       UserInterfaceLib
                                       GameStatesLib
                                       ASROB::RobotInterfaces
                                       ROBOTICSLAB::ColorDebug)

if(WIN32)
    target_compile_definitions(robotDevastation PUBLIC SDL_MAIN_HANDLED)
endif()

install(TARGETS robotDevastation
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT rd_game)

if(BUNDLE_DEPS)
    if(NOT DEFINED YARP_DEVICES_INSTALL)
        message(SEND_ERROR "Missing option: \"YARP_DEVICES_INSTALL\"")
    endif()

    if(WIN32)
        get_filename_component(_sdl_image_link_dir "${SDL2_IMAGE_LIBRARIES}" DIRECTORY)
        get_filename_component(_sdl_mixer_link_dir "${SDL2_MIXER_LIBRARIES}" DIRECTORY)

        set(_bundled_app robotDevastation)

        set(_bundled_libs_exported ${_sdl_image_link_dir}/libpng16-16.dll
                                   ${_sdl_mixer_link_dir}/libmpg123-0.dll)

        set(_bundled_libs ${_bundled_libs_exported}
                          ${YARP_DEVICES_INSTALL}/${CMAKE_INSTALL_LIBDIR}/asrob-yarp-devices/FakeMotorController.dll
                          ${YARP_DEVICES_INSTALL}/${CMAKE_INSTALL_LIBDIR}/asrob-yarp-devices/RobotClient.dll)

        set(_bundled_dirs ${_sdl_image_link_dir}
                          ${_sdl_mixer_link_dir}
                          ${YARP_DEVICES_INSTALL}/${CMAKE_INSTALL_LIBDIR})

        configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/fixup_bundle.cmake.in
                       ${CMAKE_BINARY_DIR}/fixup_bundle_robotDevastation.cmake @ONLY)

        install(FILES ${_bundled_libs_exported}
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT rd_game)

        install(SCRIPT ${CMAKE_BINARY_DIR}/fixup_bundle_robotDevastation.cmake
                COMPONENT rd_game)
    endif()

    install(DIRECTORY ${YARP_DEVICES_INSTALL}/
            DESTINATION .
            COMPONENT rd_game)
endif()

# Organize targets in folders (Visual Studio)
set_target_properties(robotDevastation PROPERTIES FOLDER Programs)

endif() # ENABLE_robotDevastation

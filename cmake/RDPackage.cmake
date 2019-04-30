# Set CPack variables.

# Miscellanea.
set(CPACK_PACKAGE_NAME "Robot Devastation")
set(CPACK_PACKAGE_VENDOR "Universidad Carlos III de Madrid")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Robot Devastation software for PC")
set(CPACK_PACKAGE_CONTACT "Asociación de Robótica UC3M (ASROB)")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)

# Package version.
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 3)
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_STRING ${CPACK_PACKAGE_VERSION})

if(DEFINED _CPACK_CI_VERSION)
    set(CPACK_PACKAGE_VERSION_STRING ${_CPACK_CI_VERSION})
endif()

# Configure components.
include(CPackComponent)

cpack_add_component(rd_server
                    DISPLAY_NAME "RD Server"
                    DESCRIPTION "Robot Devastation server application.")

cpack_add_component(rd_game
                    DISPLAY_NAME "RD Game"
                    DESCRIPTION "Robot Devastation client application."
                    DEPENDS resources)

cpack_add_component(docs
                    DISPLAY_NAME "Documentation"
                    DESCRIPTION "HTML generated documentation.")

cpack_add_component(resources
                    DISPLAY_NAME "Resources"
                    DESCRIPTION "Dynamically loaded game resources.")
					
cpack_add_component(runtime
                    DISPLAY_NAME "Runtime"
                    DESCRIPTION "System runtime libraries.")

# Create shortcuts for executables.

if(ENABLE_rdServer)
    list(APPEND CPACK_PACKAGE_EXECUTABLES rdServer "RD Server")
    list(APPEND CPACK_CREATE_DESKTOP_LINKS rdServer)
endif()

if(ENABLE_robotDevastation)
    list(APPEND CPACK_PACKAGE_EXECUTABLES robotDevastation "RD Game")
    list(APPEND CPACK_CREATE_DESKTOP_LINKS robotDevastation)
endif()

# NSIS-specific configuration variables.
if(WIN32 AND CMAKE_GENERATOR MATCHES "^Visual Studio")
    # Visual C++ toolchain.
    if(MSVC10)
        set(CPACK_SYSTEM_NAME v10)
    elseif(MSVC11)
        set(CPACK_SYSTEM_NAME v11)
    elseif(MSVC12)
        set(CPACK_SYSTEM_NAME v12)
    elseif(MSVC14)
        set(CPACK_SYSTEM_NAME v14)
    elseif(MSVC15)
        set(CPACK_SYSTEM_NAME v15)
    endif()

    # Architecture-dependent configuration.
    if(CMAKE_GENERATOR_PLATFORM MATCHES "x64" OR CMAKE_GENERATOR MATCHES "Win64")
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}_x86_amd64")
        set(CPACK_NSIS_INSTALL_ROOT "\\$PROGRAMFILES64\\\\ASROB UC3M")
    elseif(NOT CMAKE_GENERATOR MATCHES "ARM")
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}_x86")
        set(CPACK_NSIS_INSTALL_ROOT "\\$PROGRAMFILES32\\\\ASROB UC3M")
    endif()

    # Package name.
    set(CPACK_PACKAGE_FILE_NAME "robotDevastation_${CPACK_PACKAGE_VERSION_STRING}_${CPACK_SYSTEM_NAME}")

    # Miscellanea.
    set(CPACK_NSIS_HELP_LINK "https://github.com/asrob-uc3m/robotDevastation/issues/")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/asrob-uc3m/robotDevastation/")

    # Additional Start Menu shortcuts.
    list(APPEND CPACK_NSIS_MENU_LINKS "https://github.com/asrob-uc3m/robotDevastation" "Source code at GitHub")

    if(DOXYGEN_FOUND)
        list(APPEND CPACK_NSIS_MENU_LINKS "share/doc/${PROJECT_NAME}/html/index.html" "HTML documentation")
    endif()
endif()

if(UNIX)
    set(CPACK_DEBIAN_PACKAGE_NAME robotdevastation)
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://asrob.uc3m.es/workgroups/2017-05-28-robot-devastation.html")
    set(CPACK_DEBIAN_PACKAGE_SECTION games)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-mixer-2.0-0, libsdl2-ttf-2.0-0, libzbar0")

    find_program(LSB_RELEASE_EXEC lsb_release)

    # Detect Linux platform.
    if(LSB_RELEASE_EXEC)
        execute_process(COMMAND ${LSB_RELEASE_EXEC} -cs
                        OUTPUT_VARIABLE LINUX_PLATFORM
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

        set(CPACK_SYSTEM_NAME ${LINUX_PLATFORM})
    endif()

    # Detect architecture.
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}_amd64")
    else()
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}_i386")
    endif()

    # Package name.
    set(CPACK_PACKAGE_FILE_NAME "robotDevastation-${CPACK_PACKAGE_VERSION_STRING}.${CPACK_SYSTEM_NAME}")
endif()

# Launch CPack.
include(CPack)

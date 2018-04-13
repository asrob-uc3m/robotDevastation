if(COMMAND configure_and_install_desktop_shortcut)
    return()
endif()

function(configure_and_install_desktop_shortcut)
    set(_arg ${ARGV0})

    if("${_arg}" STREQUAL "")
        message(SEND_ERROR "Missing argument to configure_desktop_shortcut().")
        return()
    endif()

    # Retrieve and sanitize file name of provided template.
    get_filename_component(_template_name ${_arg} NAME)
    string(REGEX REPLACE "\\.in$" "" _filename ${_template_name})
    string(REGEX MATCH ".+\\.desktop$" _valid_filename ${_filename})

    if(NOT _valid_filename)
        message(SEND_ERROR "Invalid desktop file: \"${_filename}\".")
        return()
    endif()

    # Configure desktop shortcut (build tree).
    set(DESKTOP_SHORTCUT_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
    configure_file(${_arg} ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_filename})

    # Configure desktop shortcut (install tree).
    set(DESKTOP_SHORTCUT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    configure_file(${_arg} ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_filename}.install)

    # Set execute permissions on shortcut (build tree).
    file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_filename}
         DESTINATION ${CMAKE_BINARY_DIR}
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                          GROUP_READ GROUP_WRITE GROUP_EXECUTE
                          WORLD_READ WORLD_EXECUTE)

    # Find xdg-user-dir program.
    find_program(XdgUserDir_EXECUTABLE xdg-user-dir)
    set(_desktop_install_path)

    # Retrieve desktop location via xdg-user-dir or env variable.
    if(XdgUserDir_EXECUTABLE)
        execute_process(COMMAND ${XdgUserDir_EXECUTABLE} DESKTOP
                        OUTPUT_VARIABLE _output
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

        set(_desktop_install_path "${_output}")
    else()
        message(STATUS "xdg-user-dir not found, using $HOME environment variable.")
        set(_desktop_install_path "$ENV{HOME}")
    endif()

    # Install desktop shortcut.
    if(_desktop_install_path)
        message(STATUS "Installation path of desktop shortcut: ${_desktop_install_path}.")

        install(PROGRAMS ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_filename}.install
                DESTINATION "${_desktop_install_path}"
                RENAME ${_filename})
    else()
        message(STATUS "Not installing desktop shortcut.")
    endif()
endfunction()

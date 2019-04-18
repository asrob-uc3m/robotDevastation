# https://stackoverflow.com/a/25217937
get_directory_property(_has_parent PARENT_DIRECTORY)

if(_has_parent)
    if(NOT DOXYGEN_FOUND)
        return()
    endif()
else()
    cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
    project(_ NONE) # dummy project, disable compiler check
    find_package(YCM 0.10 REQUIRED)
    find_package(Doxygen REQUIRED)
endif()

if(CMAKE_GENERATOR MATCHES "^(Visual Studio|Xcode)")
    set(_dox DOX)
else()
    set(_dox dox)
endif()

set(DOXYGEN_PROJECT_NAME "")
set(DOXYGEN_DOXYGEN_PROJECT_NUMBER "")
set(DOXYGEN_PROJECT_BRIEF "")
set(DOXYGEN_REPEAT_BRIEF NO)
set(DOXYGEN_GROUP_NESTED_COMPOUNDS YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_STATIC YES)
set(DOXYGEN_EXAMPLE_PATH examples)
set(DOXYGEN_EXAMPLE_RECURSIVE YES)
set(DOXYGEN_IMAGE_PATH dox/fig)
set(DOXYGEN_HTML_TIMESTAMP YES)
set(DOXYGEN_HTML_DYNAMIC_SECTIONS YES)
set(DOXYGEN_DOT_FONTNAME FreeSans)

set(_doxygen_input doc
                   examples
                   share
                   src
                   tests)

doxygen_add_docs(${_dox} ${_doxygen_input}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

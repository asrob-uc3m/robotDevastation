set(ENABLE_rdServer ON CACHE BOOL "")
set(ENABLE_robotDevastation ON CACHE BOOL "")
set(ENABLE_tests ON CACHE BOOL "")
set(GENERATE_doc_html ON CACHE BOOL "")

if(WIN32)
    set(CPACK_BINARY_NSIS ON CACHE BOOL "")
    set(CPACK_BINARY_ZIP ON CACHE BOOL "")
endif()

set(_version_regex_pattern "^v")

if($ENV{APPVEYOR} STREQUAL "True")
    if($ENV{APPVEYOR_REPO_TAG} STREQUAL "true")
        string(REGEX REPLACE ${_version_regex_pattern} "" _tagged_version $ENV{APPVEYOR_REPO_TAG_NAME})
        set(_CPACK_CI_VERSION ${_tagged_version} CACHE STRING "")
        unset(_tagged_version)
    else()
        set(_CPACK_CI_VERSION $ENV{APPVEYOR_BUILD_VERSION} CACHE STRING "")
    endif()
endif()

unset(_version_regex_pattern)

set(ENABLE_rdServer ON CACHE BOOL "")
set(ENABLE_robotDevastation ON CACHE BOOL "")
set(ENABLE_tests ON CACHE BOOL "")
set(GENERATE_doc_html ON CACHE BOOL "")

if(WIN32)
    set(CPACK_BINARY_NSIS ON CACHE BOOL "")
    set(CPACK_BINARY_ZIP ON CACHE BOOL "")
endif()

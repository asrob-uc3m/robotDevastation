if(ENABLE_rdServer)

# Find YARP.  Point the YARP_DIR environment variable at your build.
find_package(YARP REQUIRED)

# Set up our main executable.
add_executable(rdServer main.cpp)

target_include_directories(rdServer PUBLIC ${YARP_INCLUDE_DIRS})

target_link_libraries(rdServer YARP::YARP_init
                               YARP::YARP_OS
                               ServerLib
                               UtilsLib)

target_compile_definitions(rdServer PUBLIC SDL_MAIN_HANDLED)

install(TARGETS rdServer DESTINATION ${CMAKE_INSTALL_BIN})

endif()

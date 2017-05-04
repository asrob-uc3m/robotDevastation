if(ENABLE_rdServer)

set(KEYWORD "rdServer")

# Find YARP.  Point the YARP_DIR environment variable at your build.
find_package(YARP REQUIRED)

# Set up our main executable.
add_executable(${KEYWORD} main.cpp)

target_link_libraries(${KEYWORD} YARP::YARP_init
                                 YARP::YARP_OS
                                 ServerLib
                                 UtilsLib)

target_compile_definitions(${KEYWORD} PUBLIC SDL_MAIN_HANDLED)

install(TARGETS ${KEYWORD} DESTINATION bin)

endif()

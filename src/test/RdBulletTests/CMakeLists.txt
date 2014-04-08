
cmake_minimum_required(VERSION 2.6)

IF (ENABLE_RdBulletTests)

include_directories(${RD_INCLUDE_DIRS})

add_executable(RdBulletBaseTest RdBulletBaseTest.cpp)
target_link_libraries(RdBulletBaseTest RdBulletLib)

ENDIF (ENABLE_RdBulletTests)

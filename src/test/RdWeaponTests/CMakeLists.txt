
cmake_minimum_required(VERSION 2.6)

IF (ENABLE_RdWeaponTests)

include_directories(${RD_INCLUDE_DIRS})

add_executable(RdWeaponBaseTest RdWeaponBaseTest.cpp)
target_link_libraries(RdWeaponBaseTest RdWeaponLib)

ENDIF (ENABLE_RdWeaponTests)

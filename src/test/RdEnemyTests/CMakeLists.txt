
cmake_minimum_required(VERSION 2.6)

IF (ENABLE_RdEnemyTests)

include_directories(${RD_INCLUDE_DIRS})

add_executable(RdEnemyBaseTest RdEnemyBaseTest.cpp)
target_link_libraries(RdEnemyBaseTest  ${RD_LIBRARIES})

ENDIF (ENABLE_RdEnemyTests)

# Testing things #########################################################################################
add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${RD_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})

add_executable(testRdAudioManager testRdAudioManager.cpp)
target_link_libraries(testRdAudioManager ${RD_LIBRARIES} RdMusicLib gtest gtest_main)

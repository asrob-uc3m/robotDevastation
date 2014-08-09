# Testing things #########################################################################################
add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${RD_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})

add_executable(testRdAudioManager testRdAudioManager.cpp)
target_link_libraries(testRdAudioManager ${RD_LIBRARIES} RdMusicLib gtest gtest_main)

add_executable(testSDLTextDisplay testSDLTextDisplay.cpp)
target_link_libraries(testSDLTextDisplay ${RD_LIBRARIES} SDL SDL_image SDL_ttf)

add_executable(testMentalMap testMentalMap.cpp)
target_link_libraries(testMentalMap ${RD_LIBRARIES} RdMentalMapLib gtest gtest_main)

add_executable(testHUD testHUD.cpp)
target_link_libraries(testHUD ${RD_LIBRARIES} SDL SDL_image SDL_ttf gtest gtest_main)

# Testing things #########################################################################################
add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${RD_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})

# UI related
add_executable(testRdAudioManager testRdAudioManager.cpp)
target_link_libraries(testRdAudioManager ${RD_LIBRARIES} RdMusicLib gtest gtest_main)

add_executable(testSDLTextDisplay testSDLTextDisplay.cpp)
target_link_libraries(testSDLTextDisplay ${RD_LIBRARIES} SDL SDL_image SDL_ttf)

add_executable(testHUD testHUD.cpp)
target_link_libraries(testHUD ${RD_LIBRARIES} SDL SDL_image SDL_ttf gtest gtest_main)

# Input manager related
add_executable(testRdInputManager testRdInputManager.cpp)
target_link_libraries(testRdInputManager ${RD_LIBRARIES} RdInputLib gtest gtest_main)

add_executable(testMockupInputManager testMockupInputManager.cpp)
target_link_libraries(testMockupInputManager ${RD_LIBRARIES} RdInputLib gtest gtest_main)

# Mental Map related
add_executable(testMentalMap testMentalMap.cpp)
target_link_libraries(testMentalMap ${RD_LIBRARIES} RdMentalMapLib gtest gtest_main)

# Network manager related
add_executable(testRdYarpNetworkManager testRdYarpNetworkManager.cpp)
target_link_libraries(testRdYarpNetworkManager ${RD_LIBRARIES} RdNetworkLib gtest gtest_main)

add_executable(testMockupNetworkManager testMockupNetworkManager.cpp)
target_link_libraries(testMockupNetworkManager ${RD_LIBRARIES} RdNetworkLib gtest gtest_main)

# Image manager related
add_executable(testRdImageManager testRdImageManager.cpp)
target_link_libraries(testRdImageManager ${RD_LIBRARIES} RdImageLib gtest gtest_main)

add_executable(testRdYarpImageManager testRdYarpImageManager.cpp)
target_link_libraries(testRdYarpImageManager ${RD_LIBRARIES} RdImageLib gtest gtest_main)

# State machine related
add_executable(testFSM testFSM.cpp)
target_link_libraries(testFSM ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

add_executable(testFSMBuilder testFSMBuilder.cpp)
target_link_libraries(testFSMBuilder ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

# Testing things #########################################################################################
set(gtest_force_shared_crt ON CACHE INTERNAL "") # needed for Windows
add_subdirectory(gtest-1.7.0)
enable_testing()

find_package(YARP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${RD_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
link_directories(${RD_LINK_DIRS})
add_definitions(-DSDL_MAIN_HANDLED) # needed for Windows

# UI related
add_executable(testSDLTextDisplay testSDLTextDisplay.cpp)
target_link_libraries(testSDLTextDisplay ${RD_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_executable(testInitScreen testInitScreen.cpp)
target_link_libraries(testInitScreen ${RD_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} gtest gtest_main)

add_executable(testGameScreen testGameScreen.cpp)
target_link_libraries(testGameScreen ${RD_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} gtest gtest_main)

add_executable(testDeadScreen testDeadScreen.cpp)
target_link_libraries(testDeadScreen ${RD_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} gtest gtest_main)

add_executable(testMockScreen testMockScreen.cpp)
target_link_libraries(testMockScreen ${RD_LIBRARIES})

add_executable(testSDLScreenManager testSDLScreenManager.cpp)
target_link_libraries(testSDLScreenManager ${RD_LIBRARIES} UserInterfaceLib)

# Audio manager related
add_executable(testMockAudioManager testMockAudioManager.cpp)
target_link_libraries(testMockAudioManager ${RD_LIBRARIES} ${SDL2_LIBRARY} MusicLib gtest gtest_main)

add_executable(testSDLAudioManager testSDLAudioManager.cpp)
target_link_libraries(testSDLAudioManager ${RD_LIBRARIES} ${SDL2_LIBRARY} MusicLib gtest gtest_main)

# Input manager related
add_executable(testInputManager testInputManager.cpp)
target_link_libraries(testInputManager ${RD_LIBRARIES} InputLib gtest gtest_main)

add_executable(testMockInputManager testMockInputManager.cpp)
target_link_libraries(testMockInputManager ${RD_LIBRARIES} ${SDL2_LIBRARY} InputLib gtest gtest_main)

# Mental Map related
add_executable(testMentalMap testMentalMap.cpp)
target_link_libraries(testMentalMap ${RD_LIBRARIES} ${SDL2_LIBRARY} MentalMapLib gtest gtest_main)

# Network manager related
add_executable(testYarpNetworkManager testYarpNetworkManager.cpp)
target_link_libraries(testYarpNetworkManager ${RD_LIBRARIES} NetworkLib gtest gtest_main)

add_executable(testMockNetworkManager testMockNetworkManager.cpp)
target_link_libraries(testMockNetworkManager ${RD_LIBRARIES} ${SDL2_LIBRARY} NetworkLib gtest gtest_main)

# Image manager related
add_executable(testYarpImageManager testYarpImageManager.cpp)
target_link_libraries(testYarpImageManager ${RD_LIBRARIES} ImageLib gtest gtest_main)

add_executable(testMockImageManager testMockImageManager.cpp)
target_link_libraries(testMockImageManager ${RD_LIBRARIES} ImageLib gtest gtest_main)

add_executable(testProcessorImageEventListener testProcessorImageEventListener.cpp)
target_link_libraries(testProcessorImageEventListener ${RD_LIBRARIES} ${SDL2_LIBRARY} ImageLib gtest gtest_main)

# Robot manager related
add_executable(testMockRobotManager testMockRobotManager.cpp)
target_link_libraries(testMockRobotManager ${RD_LIBRARIES} ${SDL2_LIBRARY} RobotLib gtest gtest_main)

# State machine related
add_executable(testFSM testFSM.cpp)
target_link_libraries(testFSM ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

add_executable(testFSMBuilder testFSMBuilder.cpp)
target_link_libraries(testFSMBuilder ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

# Game States related
add_executable(testInitState testInitState.cpp)
target_link_libraries(testInitState ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

add_executable(testGameState testGameState.cpp)
target_link_libraries(testGameState ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

add_executable(testDeadState testDeadState.cpp)
target_link_libraries(testDeadState ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

add_executable(testRobotDevastation testRobotDevastation.cpp)
target_link_libraries(testRobotDevastation ${RD_LIBRARIES} StateMachineLib gtest gtest_main)

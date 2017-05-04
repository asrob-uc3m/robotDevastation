# Testing things #########################################################################################
set(gtest_force_shared_crt ON CACHE INTERNAL "") # needed for Windows
add_subdirectory(gtest-1.7.0)
enable_testing()

find_package(YARP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_definitions(-DSDL_MAIN_HANDLED) # needed for Windows

# UI related
add_executable(testSDLTextDisplay testSDLTextDisplay.cpp)
target_link_libraries(testSDLTextDisplay YARP::YARP_OS ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})
target_include_directories(testSDLTextDisplay PUBLIC ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(testInitScreen testInitScreen.cpp)
target_link_libraries(testInitScreen YARP::YARP_OS UserInterfaceLib gtest_main)

add_executable(testGameScreen testGameScreen.cpp)
target_link_libraries(testGameScreen YARP::YARP_OS YARP::YARP_sig MentalMapLib UserInterfaceLib gtest_main)

add_executable(testDeadScreen testDeadScreen.cpp)
target_link_libraries(testDeadScreen YARP::YARP_OS YARP::YARP_sig UserInterfaceLib ImageLib UtilsLib gtest_main)

add_executable(testMockScreen testMockScreen.cpp)
target_link_libraries(testMockScreen YARP::YARP_OS UserInterfaceLib UtilsLib)

add_executable(testSDLScreenManager testSDLScreenManager.cpp)
target_link_libraries(testSDLScreenManager YARP::YARP_OS UserInterfaceLib)

# Audio manager related
add_executable(testMockAudioManager testMockAudioManager.cpp)
target_link_libraries(testMockAudioManager YARP::YARP_OS MusicLib gtest_main)

add_executable(testSDLAudioManager testSDLAudioManager.cpp)
target_link_libraries(testSDLAudioManager YARP::YARP_OS MusicLib gtest_main)

# Input manager related
add_executable(testInputManager testInputManager.cpp)
target_link_libraries(testInputManager InputLib MusicLib UtilsLib ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES} gtest_main)
target_include_directories(testInputManager PUBLIC ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(testMockInputManager testMockInputManager.cpp)
target_link_libraries(testMockInputManager InputLib gtest_main)

# Mental Map related
add_executable(testMentalMap testMentalMap.cpp)
target_link_libraries(testMentalMap MentalMapLib MusicLib gtest_main)

# Network manager related
add_executable(testYarpNetworkManager testYarpNetworkManager.cpp)
target_link_libraries(testYarpNetworkManager YARP::YARP_OS NetworkLib ServerLib UtilsLib gtest_main)

add_executable(testMockNetworkManager testMockNetworkManager.cpp)
target_link_libraries(testMockNetworkManager MentalMapLib NetworkLib gtest_main)

# Image manager related
add_executable(testYarpImageManager testYarpImageManager.cpp)
target_link_libraries(testYarpImageManager YARP::YARP_OS YARP::YARP_dev ImageLib UtilsLib gtest_main)

add_executable(testMockImageManager testMockImageManager.cpp)
target_link_libraries(testMockImageManager YARP::YARP_OS YARP::YARP_dev YARP::YARP_sig ImageLib gtest_main)

add_executable(testProcessorImageEventListener testProcessorImageEventListener.cpp)
target_link_libraries(testProcessorImageEventListener YARP::YARP_OS YARP::YARP_sig ImageLib MentalMapLib gtest_main)

# Robot manager related
add_executable(testMockRobotManager testMockRobotManager.cpp)
target_link_libraries(testMockRobotManager RobotLib gtest_main)

# State machine related
add_executable(testFSM testFSM.cpp)
target_link_libraries(testFSM YARP::YARP_OS StateMachineLib UtilsLib gtest_main)

add_executable(testFSMBuilder testFSMBuilder.cpp)
target_link_libraries(testFSMBuilder YARP::YARP_OS StateMachineLib UtilsLib gtest_main)

# Game States related
add_executable(testInitState testInitState.cpp)
target_link_libraries(testInitState YARP::YARP_OS StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)

add_executable(testGameState testGameState.cpp)
target_link_libraries(testGameState YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)

add_executable(testDeadState testDeadState.cpp)
target_link_libraries(testDeadState YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)

add_executable(testRobotDevastation testRobotDevastation.cpp)
target_link_libraries(testRobotDevastation YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)

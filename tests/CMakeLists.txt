# Authors: see AUTHORS.md at project root.
# CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
# URL: https://github.com/asrob-uc3m/robotDevastation

find_package(GTestSources 1.6.0 QUIET)

if(NOT GTestSources_FOUND AND (NOT DEFINED ENABLE_tests OR ENABLE_tests))
    message(WARNING "GTestSources package not found, disabling tests")
endif()

cmake_dependent_option(ENABLE_tests "Enable/disable unit tests" ON
                       GTestSources_FOUND OFF)

if(ENABLE_tests)
    
set(gtest_force_shared_crt ON CACHE INTERNAL "") # needed for Windows
add_subdirectory(${GTestSources_SOURCE_DIR} ${CMAKE_BINARY_DIR}/gtest)

find_package(YARP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${GTestSources_INCLUDE_DIR})

if(WIN32)
    add_definitions(-DSDL_MAIN_HANDLED)
endif()

# UI related
add_executable(testSDLTextDisplay testSDLTextDisplay.cpp)
target_link_libraries(testSDLTextDisplay YARP::YARP_OS ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})
target_include_directories(testSDLTextDisplay PUBLIC ${YARP_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(testInitScreen testInitScreen.cpp)
target_link_libraries(testInitScreen YARP::YARP_OS UserInterfaceLib gtest_main)
target_include_directories(testInitScreen PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testGameScreen testGameScreen.cpp)
target_link_libraries(testGameScreen YARP::YARP_OS YARP::YARP_sig MentalMapLib UserInterfaceLib UtilsLib gtest_main)
target_include_directories(testGameScreen PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testDeadScreen testDeadScreen.cpp)
target_link_libraries(testDeadScreen YARP::YARP_OS YARP::YARP_sig UserInterfaceLib ImageLib UtilsLib gtest_main)
target_include_directories(testDeadScreen PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testMockScreen testMockScreen.cpp)
target_link_libraries(testMockScreen YARP::YARP_OS UserInterfaceLib UtilsLib)
target_include_directories(testMockScreen PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testSDLScreenManager testSDLScreenManager.cpp)
target_link_libraries(testSDLScreenManager YARP::YARP_OS UserInterfaceLib)
target_include_directories(testSDLScreenManager PUBLIC ${YARP_INCLUDE_DIRS})

# Audio manager related
add_executable(testMockAudioManager testMockAudioManager.cpp)
target_link_libraries(testMockAudioManager YARP::YARP_OS MusicLib gtest_main)
target_include_directories(testMockAudioManager PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testSDLAudioManager testSDLAudioManager.cpp)
target_link_libraries(testSDLAudioManager YARP::YARP_OS MusicLib gtest_main)
target_include_directories(testSDLAudioManager PUBLIC ${YARP_INCLUDE_DIRS})

# Input manager related
add_executable(testInputManager testInputManager.cpp)
target_link_libraries(testInputManager InputLib MusicLib ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES} ROBOTICSLAB::ColorDebug gtest_main)
target_include_directories(testInputManager PUBLIC ${YARP_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(testMockInputManager testMockInputManager.cpp)
target_link_libraries(testMockInputManager InputLib gtest_main)

# Mental Map related
add_executable(testMentalMap testMentalMap.cpp)
target_link_libraries(testMentalMap MentalMapLib MusicLib UtilsLib gtest_main)

# Network manager related
add_executable(testYarpNetworkManager testYarpNetworkManager.cpp)
target_link_libraries(testYarpNetworkManager YARP::YARP_OS NetworkLib ServerLib ROBOTICSLAB::ColorDebug gtest_main)
target_include_directories(testYarpNetworkManager PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testMockNetworkManager testMockNetworkManager.cpp)
target_link_libraries(testMockNetworkManager MentalMapLib NetworkLib UtilsLib gtest_main)

# Image manager related
add_executable(testYarpImageManager testYarpImageManager.cpp)
target_link_libraries(testYarpImageManager YARP::YARP_OS YARP::YARP_dev ImageLib ROBOTICSLAB::ColorDebug gtest_main)
target_include_directories(testYarpImageManager PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testMockImageManager testMockImageManager.cpp)
target_link_libraries(testMockImageManager YARP::YARP_OS YARP::YARP_dev YARP::YARP_sig ImageLib gtest_main)
target_include_directories(testMockImageManager PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testProcessorImageEventListener testProcessorImageEventListener.cpp)
target_link_libraries(testProcessorImageEventListener YARP::YARP_OS YARP::YARP_sig ImageLib MentalMapLib UtilsLib gtest_main)
target_include_directories(testProcessorImageEventListener PUBLIC ${YARP_INCLUDE_DIRS})

# Robot manager related
add_executable(testMockRobotManager testMockRobotManager.cpp)
target_link_libraries(testMockRobotManager RobotLib gtest_main)

# State machine related
add_executable(testFSM testFSM.cpp)
target_link_libraries(testFSM YARP::YARP_OS YARP::YARP_init StateMachineLib ROBOTICSLAB::ColorDebug gtest_main)
target_include_directories(testFSM PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testFSMBuilder testFSMBuilder.cpp)
target_link_libraries(testFSMBuilder YARP::YARP_OS YARP::YARP_init StateMachineLib ROBOTICSLAB::ColorDebug gtest_main)
target_include_directories(testFSMBuilder PUBLIC ${YARP_INCLUDE_DIRS})

# Game States related
add_executable(testInitState testInitState.cpp)
target_link_libraries(testInitState YARP::YARP_OS StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)
target_include_directories(testInitState PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testGameState testGameState.cpp)
target_link_libraries(testGameState YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)
target_include_directories(testGameState PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testDeadState testDeadState.cpp)
target_link_libraries(testDeadState YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)
target_include_directories(testDeadState PUBLIC ${YARP_INCLUDE_DIRS})

add_executable(testRobotDevastation testRobotDevastation.cpp)
target_link_libraries(testRobotDevastation YARP::YARP_OS YARP::YARP_sig StateMachineLib GameStatesLib NetworkLib ImageLib InputLib MentalMapLib RobotLib MusicLib UserInterfaceLib UtilsLib gtest_main)
target_include_directories(testRobotDevastation PUBLIC ${YARP_INCLUDE_DIRS})

# Organize targets in folders (Visual Studio)
set_target_properties(testSDLTextDisplay
                      testInitScreen
                      testGameScreen
                      testDeadScreen
                      testMockScreen
                      testSDLScreenManager
                      testMockAudioManager
                      testSDLAudioManager
                      testInputManager
                      testMockInputManager
                      testMentalMap
                      testYarpNetworkManager
                      testMockNetworkManager
                      testYarpImageManager
                      testMockImageManager
                      testProcessorImageEventListener
                      testMockRobotManager
                      testFSM
                      testFSMBuilder
                      testInitState
                      testGameState
                      testDeadState
                      testRobotDevastation
                      PROPERTIES FOLDER Tests)

set_target_properties(gtest gtest_main PROPERTIES FOLDER Tests/gtest)

else()

set(ENABLE_tests OFF CACHE BOOL "Enable/disable unit tests" FORCE)

endif() # ENABLE_tests

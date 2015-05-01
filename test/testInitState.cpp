#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"
#include "InitState.hpp"

#include "MockupNetworkManager.hpp"
#include "RdMockupImageManager.hpp"
#include "MockupInputManager.hpp"
#include "RdMentalMap.hpp"
#include "RdMockupRobotManager.hpp"
#include "MockupAudioManager.hpp"

#include <yarp/os/Network.h>

using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class InitStateTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Start YARP network
            yarp::os::Network::init();

            //-- Register managers to be used:
            MockupNetworkManager::RegisterManager();
            RdMockupImageManager::RegisterManager();
            MockupInputManager::RegisterManager();
            MockupAudioManager::RegisterManager();

            //-- Create managers
            networkManager = RdNetworkManager::getNetworkManager("MOCKUP");
            mockupNetworkManager = dynamic_cast<MockupNetworkManager *>(networkManager);
            ASSERT_NE((RdNetworkManager*) NULL, networkManager);
            ASSERT_NE((MockupNetworkManager*) NULL, mockupNetworkManager);

            imageManager = RdImageManager::getImageManager("MOCKUP");
            mockupImageManager = dynamic_cast<RdMockupImageManager *>(imageManager);
            ASSERT_NE((RdImageManager*) NULL, imageManager);
            ASSERT_NE((RdMockupImageManager*) NULL, mockupImageManager);

            inputManager = RdInputManager::getInputManager("MOCKUP");
            mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);
            ASSERT_NE((RdInputManager*) NULL, inputManager);
            ASSERT_NE((MockupInputManager*) NULL, mockupInputManager);

            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockupAudioManager*) NULL, mockupAudioManager);

            mentalMap = RdMentalMap::getMentalMap();
            ASSERT_NE((RdMentalMap*) NULL, mentalMap);

            mockupRobotManager = new RdMockupRobotManager("MOCKUP");
            robotManager = (RdRobotManager *) mockupRobotManager;
            ASSERT_NE((RdMockupRobotManager*) NULL, mockupRobotManager);
            ASSERT_NE((RdRobotManager*) NULL, robotManager);
        }

        virtual void TearDown()
        {
            //-- Close YARP network
            yarp::os::Network::fini();


            //-- Delete things
            RdNetworkManager::destroyNetworkManager();
            networkManager = NULL;
            mockupNetworkManager = NULL;
            RdImageManager::destroyImageManager();
            imageManager = NULL;
            mockupImageManager = NULL;
            RdInputManager::destroyInputManager();
            AudioManager::destroyAudioManager();

            RdMentalMap::destroyMentalMap();

            delete mockupRobotManager;
            mockupRobotManager = NULL;

        }

    protected:
        FiniteStateMachine *fsm;

        RdNetworkManager * networkManager;
        MockupNetworkManager * mockupNetworkManager;

        RdImageManager * imageManager;
        RdMockupImageManager * mockupImageManager;

        RdInputManager * inputManager;
        MockupInputManager * mockupInputManager;

        AudioManager * audioManager;
        MockupAudioManager * mockupAudioManager;

        RdMentalMap * mentalMap;

        RdMockupRobotManager * mockupRobotManager;
        RdRobotManager * robotManager;
};

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(InitStateTest, InitStateWorksCorrectly )
{
    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager));
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_NE(-1, init_state_id);
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::LOGIN_SUCCESSFUL));
    ASSERT_TRUE(builder.setInitialState(init_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupInputManager->isStopped());
//    ASSERT_TRUE(mockupRobotManager->isStopped());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before login (loop):
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_FALSE(mockupNetworkManager->isLoggedIn());

    ASSERT_TRUE(mockupImageManager->isStopped());

    ASSERT_FALSE(mockupInputManager->isStopped());

//    ASSERT_FALSE(mockupRobotManager->isStopped());
//    ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- When enter is pressed, the system should log in and go to next state:
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ENTER));

    //-- Check that it has logged in and it is in the next state (cleanup):
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());

    ASSERT_FALSE(mockupImageManager->isStopped());

    ASSERT_FALSE(mockupInputManager->isStopped());

//    ASSERT_FALSE(mockupRobotManager->isStopped());
//    ASSERT_TRUE(mockupRobotManager->isConnected());

}

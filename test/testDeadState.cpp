#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"
#include "DeadState.hpp"

#include "MockupNetworkManager.hpp"
#include "RdMockupImageManager.hpp"
#include "MockupInputManager.hpp"
#include "RdMentalMap.hpp"
#include "RdMockupRobotManager.hpp"
#include "MockupAudioManager.hpp"
#include "MockupState.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

using namespace rd;

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This is required since MockupStates are used (and require yarp ports to be open)

class DeadStateTestEnvironment : public testing::Environment
{
    public:
        DeadStateTestEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();
        }

        virtual void TearDown()
        {
            yarp::os::Network::fini();
        }


    private:
        int argc;
        char ** argv;

};

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class DeadStateTest : public testing::Test
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
            mockupAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);

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
TEST_F(DeadStateTest, DeadStateGoesToRespawn)
{
    //-- Create fsm with DeadState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager));
    ASSERT_NE(-1, dead_state_id);
    int game_state_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, game_state_id);
    int exit_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED));
    ASSERT_TRUE(builder.addTransition(dead_state_id, exit_state_id, DeadState::EXIT_SELECTED));
    ASSERT_TRUE(builder.setInitialState(dead_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    // Player is dead
    // Stuff is enabled
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in dead state before time runs out (setup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_TRUE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When enter is pressed, but the countdown is still active, input is ignored
    yarp::os::Time::delay(0.5);
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ENTER));
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When time is up, and enter is pressed, the system should go to respawn state:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ENTER));
    yarp::os::Time::delay(0.5);

    //-- Check that it has restored things (health, enable stuff)
    //-- and it is in the game state (cleanup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that deadState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

}

TEST_F(DeadStateTest, DeadStateGoesToLogout)
{
    //-- Create fsm with DeadState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager));
    ASSERT_NE(-1, dead_state_id);
    int game_state_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, game_state_id);
    int exit_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED));
    ASSERT_TRUE(builder.addTransition(dead_state_id, exit_state_id, DeadState::EXIT_SELECTED));
    ASSERT_TRUE(builder.setInitialState(dead_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    // Player is dead
    // Stuff is enabled
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in dead state before time runs out (setup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_TRUE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When enter is pressed, but the countdown is still active, input is ignored
    yarp::os::Time::delay(0.5);
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ENTER));
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When time is up, and esc is pressed, the system should exit the game:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ESCAPE));
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_FALSE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that end state is active
    ASSERT_EQ(exit_state_id, fsm->getCurrentState());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new DeadStateTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

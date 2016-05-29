#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"

#include "MockupNetworkManager.hpp"
#include "RdMockupImageManager.hpp"
#include "MockupInputManager.hpp"
#include "RdMentalMap.hpp"
#include "RdMockupRobotManager.hpp"
#include "MockupAudioManager.hpp"

//-- Game FSM
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "InitState.hpp"
#include "GameState.hpp"
#include "DeadState.hpp"


using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RobotDevastationTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Start YARP network
            yarp::os::Network::init();

            //-- Init input manager
            MockupInputManager::RegisterManager();
            inputManager = RdInputManager::getInputManager("MOCKUP");
            mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);
            ASSERT_NE((RdInputManager*) NULL, inputManager);
            ASSERT_NE((MockupInputManager*) NULL, mockupInputManager);

            //-- Init sound
            MockupAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockupAudioManager*) NULL, mockupAudioManager);
            mockupAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);
            mockupAudioManager->load("shoot","shoot", AudioManager::FX);
            mockupAudioManager->load("explosion","explosion", AudioManager::FX);
            mockupAudioManager->load("noAmmo","noAmmo", AudioManager::FX);
            mockupAudioManager->load("reload","reload", AudioManager::FX);

            //-- Init robot:
            mockupRobotManager = new RdMockupRobotManager("MOCKUP");
            robotManager = (RdRobotManager *) mockupRobotManager;
            ASSERT_NE((RdMockupRobotManager*) NULL, mockupRobotManager);
            ASSERT_NE((RdRobotManager*) NULL, robotManager);

            //-- Init image manager
            RdMockupImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager("MOCKUP");
            mockupImageManager = dynamic_cast<RdMockupImageManager *>(imageManager);
            ASSERT_NE((RdImageManager*) NULL, imageManager);
            ASSERT_NE((RdMockupImageManager*) NULL, mockupImageManager);

            //-- Init mental map
            mentalMap = RdMentalMap::getMentalMap();
            ASSERT_NE((RdMentalMap*) NULL, mentalMap);
            ASSERT_TRUE(mentalMap->configure(1));

            std::vector<RdPlayer> players;
            players.push_back(RdPlayer(1,"test_player",100,100,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(RdWeapon("Default gun", 10, 5));

            //-- Init network manager
            MockupNetworkManager::RegisterManager();
            networkManager = RdNetworkManager::getNetworkManager("MOCKUP");
            mockupNetworkManager = dynamic_cast<MockupNetworkManager *>(networkManager);
            ASSERT_NE((RdNetworkManager*) NULL, networkManager);
            ASSERT_NE((MockupNetworkManager*) NULL, mockupNetworkManager);
            networkManager->configure("player", players[0]);

            //-- Setup Game FSM
            StateMachineBuilder builder;
            builder.setDirectorType("YARP");

            //-- Create states
            int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                               robotManager, audioManager));
            int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                               robotManager, audioManager));
            int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                               mentalMap, robotManager, audioManager));
            int end_state_id = builder.addState(State::getEndState());

            //-- Add transitions to other states
            builder.addTransition(init_state_id, game_state_id, InitState::LOGIN_SUCCESSFUL);
            builder.addTransition(game_state_id, dead_state_id, GameState::KILLED);
            builder.addTransition(game_state_id, end_state_id, GameState::EXIT_REQUESTED);
            builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED);
            builder.addTransition(dead_state_id, end_state_id, DeadState::EXIT_SELECTED);

            //-- Set initial state
            builder.setInitialState(init_state_id);
            fsm = builder.buildStateMachine();
            ASSERT_NE((FiniteStateMachine*)NULL, fsm);
        }

        virtual void TearDown()
        {
            RdInputManager::destroyInputManager();
            inputManager = NULL;

            RdNetworkManager::destroyNetworkManager();
            networkManager = NULL;

            //-- Closing audio system:
            AudioManager::destroyAudioManager();
            audioManager = NULL;

            //-- Closing mental map:
            RdMentalMap::destroyMentalMap();
            mentalMap = NULL;

            //-- Close img related ports:
            RdImageManager::destroyImageManager();
            imageManager = NULL;

            //-- Close robot:
            delete robotManager;
            robotManager = NULL;

            //-- Delete FSM:
            delete gameFSM;
            gameFSM = NULL;

            //-- Close YARP network
            yarp::os::Network::fini();
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
TEST_F(RobotDevastationTest, RobotDevastationWorks)
{
    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_FALSE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before login (loop):

    //yarp::os::Time::delay(1);
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_FALSE(mockupNetworkManager->isLoggedIn());

    ASSERT_TRUE(mockupImageManager->isStopped());

    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());

    ASSERT_FALSE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

    //-- When enter is pressed, the system should log in and go to next state:
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ENTER));
    yarp::os::Time::delay(0.5);

    //-- Check that it has logged in and it is in the next state (cleanup):
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());

    ASSERT_TRUE(mockupImageManager->isStopped());

    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());

    ASSERT_TRUE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

}

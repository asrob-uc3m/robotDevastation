#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"
#include "InitState.hpp"
#include "GameState.hpp"

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
class GameStateTest : public testing::Test
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
            RdPlayer me = RdPlayer(0, "TEST_PLAYER", 100, 100, 0, 0);

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
TEST_F(GameStateTest, GameStateGameFlowIsCorrect)
{
    //-- Create fsm with GameState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager));
    ASSERT_NE(-1, game_state_id);
    int dead_state_id = builder.addState(new MockupState(1));
    ASSERT_NE(-1, dead_state_id);
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(game_state_id, dead_state_id, GameState::KILLED));
    ASSERT_TRUE(builder.addTransition(game_state_id, end_state_id, GameState::QUIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(game_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
//    ASSERT_FALSE(mockupRobotManager->isStopped());
//    ASSERT_TRUE(mockupRobotManager->isConnected());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen just after the fsm starts (after setup)
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
//    ASSERT_FALSE(mockupRobotManager->isStopped());
//    ASSERT_TRUE(mockupRobotManager->isConnected());

    //-- Testing game flow
    //-----------------------------------------------------------------------------
    //-- If my robot is hit, health decreases
    ASSERT_TRUE(mockupNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    ASSERT_EQ(50, mentalMap->getMyself().getHealth());

    //-- If I send move commands, robot moves
    //-- (Insert code here)

    //-- If I shoot with no target in the scope, the enemies life is kept equal
    //-- (Insert code here)

    //-- If I shoot all ammo, I cannot shoot until reloading
    //-- (Insert code here)

    //-- After reloading, I can shoot again
    //-- (Insert code here)

    //-- If I hit other robot, other robot health decreases
    //-- (Insert code here)

    //-- If I lose all health, game is over
    ASSERT_TRUE(mockupNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    ASSERT_EQ(0, mentalMap->getMyself().getHealth());


    //-- Check things that should occur before going to dead state (cleanup)
    //------------------------------------------------------------------------------
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
}

TEST_F(GameStateTest, GameStateQuitsWhenRequested )
{
    ASSERT_TRUE(false);
}

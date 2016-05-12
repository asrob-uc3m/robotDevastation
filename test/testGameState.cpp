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
#include "MockupState.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

using namespace rd;

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This is required since MockupStates are used (and require yarp ports to be open)

class GameStateTestEnvironment : public testing::Environment
{
    public:
        GameStateTestEnvironment(int argc, char ** argv)
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

            imageManager = RdImageManager::getImageManager("MOCKUP");
            mockupImageManager = dynamic_cast<RdMockupImageManager *>(imageManager);
            ASSERT_NE((RdImageManager*) NULL, imageManager);
            ASSERT_NE((RdMockupImageManager*) NULL, mockupImageManager);
            //-- Load test images
            yarp::sig::file::read(test_frame_no_target, FRAME_NO_TARGET_PATH);
            yarp::sig::file::read(test_frame_with_target, FRAME_WITH_TARGET_PATH);

            inputManager = RdInputManager::getInputManager("MOCKUP");
            mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);
            ASSERT_NE((RdInputManager*) NULL, inputManager);
            ASSERT_NE((MockupInputManager*) NULL, mockupInputManager);

            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockupAudioManager*) NULL, mockupAudioManager);
            ASSERT_TRUE(mockupAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC));
            ASSERT_TRUE(mockupAudioManager->load("shoot", "shoot", AudioManager::FX));
            ASSERT_TRUE(mockupAudioManager->load("noAmmo", "noAmmo", AudioManager::FX));
            ASSERT_TRUE(mockupAudioManager->load("reload", "reload", AudioManager::FX));

            mentalMap = RdMentalMap::getMentalMap();
            ASSERT_NE((RdMentalMap*) NULL, mentalMap);
            mentalMap->addWeapon(RdWeapon("Machine gun", 10, MAX_AMMO));
            ASSERT_TRUE(mentalMap->configure(0));
            //-- Insert players for testing
            std::vector<RdPlayer> players;
            players.push_back(RdPlayer(1,"enemy", MAX_HEALTH, MAX_HEALTH, 0, 0) );
            ASSERT_TRUE(mockupNetworkManager->setPlayerData(players));
            players.push_back(RdPlayer(0,"test_player", MAX_HEALTH, MAX_HEALTH, 0, 0));
            ASSERT_TRUE(mentalMap->updatePlayers(players));

            mockupRobotManager = new RdMockupRobotManager("MOCKUP");
            robotManager = (RdRobotManager *) mockupRobotManager;
            ASSERT_NE((RdMockupRobotManager*) NULL, mockupRobotManager);
            ASSERT_NE((RdRobotManager*) NULL, robotManager);

            //-- Setup managers to the required initial state:
            //-- Note: For simplicity, I'm using InitState here and manually calling
            //-- the correct initialization sequence. The testInitState allows to test
            //-- if InitState works correctly.
            State * initState = new InitState(networkManager, imageManager, inputManager,
                                              mentalMap, robotManager, audioManager);
            initState->setup();
            dynamic_cast<RdInputEventListener *>(initState)->onKeyUp(MockupKey(RdKey::KEY_ENTER));
            initState->loop();
            initState->cleanup();
            delete initState;
            initState = NULL;
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

    static const int MAX_HEALTH;
    static const int MAX_AMMO;

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

        RdImage test_frame_no_target;
        RdImage test_frame_with_target;

        static const std::string FRAME_NO_TARGET_PATH;
        static const std::string FRAME_WITH_TARGET_PATH;
};

const int GameStateTest::MAX_HEALTH = 100;
const int GameStateTest::MAX_AMMO = 10;
const std::string GameStateTest::FRAME_NO_TARGET_PATH = "../../share/images/test_frame_qr.ppm";
const std::string GameStateTest::FRAME_WITH_TARGET_PATH = "../../share/images/test_frame_qr_centered.ppm";

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
    yarp::os::Time::delay(0.5);

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
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- If my robot is hit, health decreases
    ASSERT_TRUE(mockupNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(50, mentalMap->getMyself().getHealth());

    //-- If I send move commands, robot moves
//    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ARROW_LEFT));

    //-- If I shoot with no target in the scope, the enemies life is kept equal
    mockupImageManager->receiveImage(test_frame_no_target);
    yarp::os::Time::delay(0.5);
    std::vector<RdPlayer> players_before = mentalMap->getPlayers();
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_SPACE));
    std::vector<RdPlayer> players_after = mentalMap->getPlayers();
    ASSERT_EQ(players_before.size(), players_after.size());
    for(int i = 0; i < players_before.size(); i++)
        EXPECT_EQ(players_before[i].getHealth(), players_after[i].getHealth());

    //-- If I shoot all ammo, I run out of ammo, and I cannot shoot until reloading
    for(int i = 0; i < GameStateTest::MAX_AMMO; i++)
        mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_SPACE));
    ASSERT_EQ(0, mentalMap->getCurrentWeapon().getCurrentAmmo());
    yarp::os::Time::delay(0.5);

    //-- After reloading, I can shoot again
    mockupInputManager->sendKeyPress(MockupKey('r'));
    ASSERT_EQ(GameStateTest::MAX_AMMO, mentalMap->getCurrentWeapon().getCurrentAmmo());

    //-- If I hit other robot, other robot health decreases
    mockupImageManager->receiveImage(test_frame_with_target);
    yarp::os::Time::delay(0.5);
    players_before = mentalMap->getPlayers();
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_SPACE));
    yarp::os::Time::delay(0.5);
    players_after = mentalMap->getPlayers();
    ASSERT_EQ(players_before.size(), players_after.size());
    for(int i = 0; i < players_before.size(); i++)
        if (players_before[i].getId() != mentalMap->getMyself().getId())
        {
            ASSERT_EQ(players_before[i].getId(), players_after[i].getId());
            EXPECT_LT(players_after[i].getHealth(), players_before[i].getHealth());
        }

    //-- If I lose all health, game is over
    ASSERT_TRUE(mockupNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    ASSERT_EQ(0, mentalMap->getMyself().getHealth());
    yarp::os::Time::delay(0.5);

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
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());
}

TEST_F(GameStateTest, GameStateQuitsWhenRequested )
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
    yarp::os::Time::delay(0.5);

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

    //-- Testing exiting game
    //-----------------------------------------------------------------------------
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- When esc is pressed, the system should exit the game:
    mockupInputManager->sendKeyPress(MockupKey(RdKey::KEY_ESCAPE));
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_FALSE(mockupNetworkManager->isLoggedIn());
    //ASSERT_FALSE(mockupRobotManager->isStopped()); //-- Not correctly implemented
    //ASSERT_FALSE(mockupRobotManager->isConnected());

    //-- Check that end state is active
    ASSERT_EQ(-1, fsm->getCurrentState()); //-- (When FSM is ended, no state is active, hence -1)
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new GameStateTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

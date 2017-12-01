// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"
#include <string>
#include <vector>

#include <yarp/sig/all.h>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "Utils.hpp"
#include "SDLUtils.hpp"
#include "InitState.hpp"
#include "GameState.hpp"
#include "MockState.hpp"

#include "MockNetworkManager.hpp"
#include "MockImageManager.hpp"
#include "MockInputManager.hpp"
#include "MentalMap.hpp"
#include "MockRobotManager.hpp"
#include "MockAudioManager.hpp"
#include "SDLScreenManager.hpp"

using namespace rd;

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This is required since MockStates are used (and require yarp ports to be open)

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

            //-- Init SDL
            //initSDL();
        }

        virtual void TearDown()
        {
            yarp::os::Network::fini();
            //cleanupSDL();
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
            //-- Find the real path to the resources with ResourceFinder
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");

            //-- Start YARP network
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();

            //-- Register managers to be used:
            MockNetworkManager::RegisterManager();
            MockImageManager::RegisterManager();
            MockInputManager::RegisterManager();
            MockAudioManager::RegisterManager();
            SDLScreenManager::RegisterManager();

            //-- Create managers
            networkManager = NetworkManager::getNetworkManager("MOCK");
            mockNetworkManager = dynamic_cast<MockNetworkManager *>(networkManager);
            ASSERT_NE((NetworkManager*) NULL, networkManager);
            ASSERT_NE((MockNetworkManager*) NULL, mockNetworkManager);

            imageManager = ImageManager::getImageManager("MOCK");
            mockImageManager = dynamic_cast<MockImageManager *>(imageManager);
            ASSERT_NE((ImageManager*) NULL, imageManager);
            ASSERT_NE((MockImageManager*) NULL, mockImageManager);
            //-- Load test images
            yarp::sig::file::read(test_frame_no_target, rf.findFileByName(FRAME_NO_TARGET_PATH));
            yarp::sig::file::read(test_frame_with_target, rf.findFileByName(FRAME_WITH_TARGET_PATH));

            inputManager = InputManager::getInputManager("MOCK");
            mockInputManager = dynamic_cast<MockInputManager *>(inputManager);
            ASSERT_NE((InputManager*) NULL, inputManager);
            ASSERT_NE((MockInputManager*) NULL, mockInputManager);

            audioManager = AudioManager::getAudioManager("MOCK");
            mockAudioManager = dynamic_cast<MockAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockAudioManager*) NULL, mockAudioManager);
            ASSERT_TRUE(mockAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC));
            ASSERT_TRUE(mockAudioManager->load("shoot", "shoot", AudioManager::FX));
            ASSERT_TRUE(mockAudioManager->load("noAmmo", "noAmmo", AudioManager::FX));
            ASSERT_TRUE(mockAudioManager->load("reload", "reload", AudioManager::FX));

            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*) NULL, mentalMap);
            mentalMap->addWeapon(Weapon("Machine gun", 10, MAX_AMMO));
            ASSERT_TRUE(mentalMap->configure(0));
            //-- Insert players for testing
            std::vector<Player> players;
            players.push_back(Player(1,"enemy", MAX_HEALTH, MAX_HEALTH, 0, 0) );
            ASSERT_TRUE(mockNetworkManager->setPlayerData(players));
            players.push_back(Player(0,"test_player", MAX_HEALTH, MAX_HEALTH, 0, 0));
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            networkManager->configure("player", players[1]);

            mockRobotManager = new MockRobotManager("MOCK");
            robotManager = (RobotManager *) mockRobotManager;
            ASSERT_NE((MockRobotManager*) NULL, mockRobotManager);
            ASSERT_NE((RobotManager*) NULL, robotManager);

            screenManager = ScreenManager::getScreenManager("SDL");
            ASSERT_NE((ScreenManager*) NULL, screenManager);
            screenManager->start();

            //-- Setup managers to the required initial state:
            //-- Note: For simplicity, I'm using InitState here and manually calling
            //-- the correct initialization sequence. The testInitState allows to test
            //-- if InitState works correctly.
            State * initState = new InitState(networkManager, imageManager, inputManager,
                                              mentalMap, robotManager, audioManager,
                                              screenManager);
            initState->setup();
            dynamic_cast<InputEventListener *>(initState)->onKeyUp(Key::KEY_ENTER);
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
            NetworkManager::destroyNetworkManager();
            networkManager = NULL;
            mockNetworkManager = NULL;
            ImageManager::destroyImageManager();
            imageManager = NULL;
            mockImageManager = NULL;
            InputManager::destroyInputManager();
            AudioManager::destroyAudioManager();

            MentalMap::destroyMentalMap();

            delete mockRobotManager;
            mockRobotManager = NULL;

            screenManager->stop();
            ScreenManager::destroyScreenManager();
        }

    static const int MAX_HEALTH;
    static const int MAX_AMMO;

    protected:
        FiniteStateMachine *fsm;

        NetworkManager * networkManager;
        MockNetworkManager * mockNetworkManager;

        ImageManager * imageManager;
        MockImageManager * mockImageManager;

        InputManager * inputManager;
        MockInputManager * mockInputManager;

        AudioManager * audioManager;
        MockAudioManager * mockAudioManager;

        MentalMap * mentalMap;

        MockRobotManager * mockRobotManager;
        RobotManager * robotManager;

        ScreenManager * screenManager;

        Image test_frame_no_target;
        Image test_frame_with_target;

        static const std::string FRAME_NO_TARGET_PATH;
        static const std::string FRAME_WITH_TARGET_PATH;
};

const int GameStateTest::MAX_HEALTH = 100;
const int GameStateTest::MAX_AMMO = 10;
const std::string GameStateTest::FRAME_NO_TARGET_PATH = "../images/test_frame_qr.ppm";
const std::string GameStateTest::FRAME_WITH_TARGET_PATH = "../images/test_frame_qr_centered.ppm";

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(GameStateTest, GameStateGameFlowIsCorrect)
{
    //-- Create fsm with GameState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    ASSERT_NE(-1, game_state_id);
    int dead_state_id = builder.addState(new MockState(1));
    ASSERT_NE(-1, dead_state_id);
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(game_state_id, dead_state_id, GameState::KILLED));
    ASSERT_TRUE(builder.addTransition(game_state_id, end_state_id, GameState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(game_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());
    yarp::os::Time::delay(0.5);

    //-- Check things that should happen just after the fsm starts (after setup)
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Testing game flow
    //-----------------------------------------------------------------------------
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- If my robot is hit, health decreases
    ASSERT_TRUE(mockNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(50, mentalMap->getMyself().getHealth());

    //-- If I send movement/camera commands, robot moves

    //-- Turn left
    mockInputManager->sendKeyDown(GameState::KEY_TURN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::LEFT, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TURN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Turn right
    mockInputManager->sendKeyDown(GameState::KEY_TURN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::RIGHT,((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TURN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Move forward
    mockInputManager->sendKeyDown(GameState::KEY_MOVE_FWD);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::FORWARD, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_MOVE_FWD);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Move backwards
    mockInputManager->sendKeyDown(GameState::KEY_MOVE_BACK);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::BACKWARDS, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_MOVE_BACK);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Pan left
    mockInputManager->sendKeyDown(GameState::KEY_PAN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_LEFT, ((MockRobotManager *)robotManager)->getCameraMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_PAN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_NONE, ((MockRobotManager *)robotManager)->getCameraMovementDirection());

    //-- Pan right
    mockInputManager->sendKeyDown(GameState::KEY_PAN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_RIGHT,((MockRobotManager *)robotManager)->getCameraMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_PAN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_NONE, ((MockRobotManager *)robotManager)->getCameraMovementDirection());

    //-- Tilt up
    mockInputManager->sendKeyDown(GameState::KEY_TILT_UP);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_UP, ((MockRobotManager *)robotManager)->getCameraMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TILT_UP);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_NONE, ((MockRobotManager *)robotManager)->getCameraMovementDirection());

    //-- Tilt down
    mockInputManager->sendKeyDown(GameState::KEY_TILT_DOWN);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_DOWN, ((MockRobotManager *)robotManager)->getCameraMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TILT_DOWN);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isCameraMoving());
    ASSERT_EQ(MockRobotManager::CAMERA_NONE, ((MockRobotManager *)robotManager)->getCameraMovementDirection());

    //-- If I shoot with no target in the scope, the enemies life is kept equal
    mockImageManager->receiveImage(test_frame_no_target);
    yarp::os::Time::delay(0.5);
    std::vector<Player> players_before = mentalMap->getPlayers();
    mockInputManager->sendKeyPress(GameState::KEY_SHOOT);
    std::vector<Player> players_after = mentalMap->getPlayers();
    ASSERT_EQ(players_before.size(), players_after.size());
    for(int i = 0; i < (int) players_before.size(); i++)
        EXPECT_EQ(players_before[i].getHealth(), players_after[i].getHealth());

    //-- If I shoot all ammo, I run out of ammo, and I cannot shoot until reloading
    for(int i = 0; i < GameStateTest::MAX_AMMO; i++)
        mockInputManager->sendKeyPress(GameState::KEY_SHOOT);
    ASSERT_EQ(0, mentalMap->getCurrentWeapon().getCurrentAmmo());
    yarp::os::Time::delay(0.5);

    //-- After reloading, I can shoot again
    mockInputManager->sendKeyPress('r');
    ASSERT_EQ(GameStateTest::MAX_AMMO, mentalMap->getCurrentWeapon().getCurrentAmmo());

    //-- If I hit other robot, other robot health decreases
    mockImageManager->receiveImage(test_frame_with_target);
    yarp::os::Time::delay(0.5);
    players_before = mentalMap->getPlayers();
    mockInputManager->sendKeyPress(GameState::KEY_SHOOT);
    yarp::os::Time::delay(0.5);
    players_after = mentalMap->getPlayers();
    ASSERT_EQ(players_before.size(), players_after.size());
    for(int i = 0; i < (int) players_before.size(); i++)
        if (players_before[i].getId() != mentalMap->getMyself().getId())
        {
            ASSERT_EQ(players_before[i].getId(), players_after[i].getId());
            EXPECT_LT(players_after[i].getHealth(), players_before[i].getHealth());
        }

    //-- If I lose all health, game is over
    ASSERT_TRUE(mockNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    ASSERT_EQ(0, mentalMap->getMyself().getHealth());
    yarp::os::Time::delay(0.5);

    //-- Check things that should occur before going to dead state (cleanup)
    //------------------------------------------------------------------------------
    // Player is dead
    // Stuff is enabled
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());
}

TEST_F(GameStateTest, GameStateQuitsWhenRequestedKeyPress)
{
    //-- Create fsm with GameState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    ASSERT_NE(-1, game_state_id);
    int dead_state_id = builder.addState(new MockState(1));
    ASSERT_NE(-1, dead_state_id);
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(game_state_id, dead_state_id, GameState::KILLED));
    ASSERT_TRUE(builder.addTransition(game_state_id, end_state_id, GameState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(game_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());
    yarp::os::Time::delay(0.5);

    //-- Check things that should happen just after the fsm starts (after setup)
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Testing exiting game
    //-----------------------------------------------------------------------------
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- When esc is pressed, the system should exit the game:
    mockInputManager->sendKeyPress(Key::KEY_ESCAPE);
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_TRUE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_FALSE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Check that end state is active
    ASSERT_EQ(-1, fsm->getCurrentState()); //-- (When FSM is ended, no state is active, hence -1)
}

TEST_F(GameStateTest, GameStateQuitsWhenRequestedWindowEvent)
{
    //-- Create fsm with GameState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    ASSERT_NE(-1, game_state_id);
    int dead_state_id = builder.addState(new MockState(1));
    ASSERT_NE(-1, dead_state_id);
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_TRUE(builder.addTransition(game_state_id, dead_state_id, GameState::KILLED));
    ASSERT_TRUE(builder.addTransition(game_state_id, end_state_id, GameState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(game_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());
    yarp::os::Time::delay(0.5);

    //-- Check things that should happen just after the fsm starts (after setup)
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Testing exiting game
    //-----------------------------------------------------------------------------
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- When the window is closed, the system should exit the game:
    mockInputManager->sendWindowEvent(WindowEvent::WINDOW_CLOSE);
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_TRUE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_FALSE(mockNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

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

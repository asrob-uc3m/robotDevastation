// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/sig/all.h>
#include <yarp/os/ResourceFinder.h>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "Utils.hpp"
#include "SDLUtils.hpp"

#include "MockNetworkManager.hpp"
#include "MockImageManager.hpp"
#include "MockInputManager.hpp"
#include "MentalMap.hpp"
#include "MockRobotManager.hpp"
#include "MockAudioManager.hpp"
#include "SDLScreenManager.hpp"

//-- Game FSM
#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "InitState.hpp"
#include "GameState.hpp"
#include "DeadState.hpp"


using namespace rd;

class RobotDevastationTestEnvironment : public testing::Environment
{
    public:
        RobotDevastationTestEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            initSDL();
        }

        virtual void TearDown()
        {
            cleanupSDL();
        }


    private:
        int argc;
        char ** argv;

};

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RobotDevastationTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Configure Resourcefinder to ind the real path to the resources
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");

            //-- Start YARP network
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();

            //-- Load test images
            yarp::sig::file::read(test_frame_no_target, rf.findFileByName(FRAME_NO_TARGET_PATH));
            yarp::sig::file::read(test_frame_with_target, rf.findFileByName(FRAME_WITH_TARGET_PATH));

            //-- Init input manager
            MockInputManager::RegisterManager();
            inputManager = InputManager::getInputManager("MOCK");
            mockInputManager = dynamic_cast<MockInputManager *>(inputManager);
            ASSERT_NE((InputManager*) NULL, inputManager);
            ASSERT_NE((MockInputManager*) NULL, mockInputManager);

            //-- Init sound
            MockAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("MOCK");
            mockAudioManager = dynamic_cast<MockAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockAudioManager*) NULL, mockAudioManager);
            mockAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);
            mockAudioManager->load("RD_DEAD","RD_DEAD", AudioManager::MUSIC);
            mockAudioManager->load("shoot","shoot", AudioManager::FX);
            mockAudioManager->load("explosion","explosion", AudioManager::FX);
            mockAudioManager->load("noAmmo","noAmmo", AudioManager::FX);
            mockAudioManager->load("reload","reload", AudioManager::FX);

            //-- Init robot:
            mockRobotManager = new MockRobotManager("MOCK");
            robotManager = (RobotManager *) mockRobotManager;
            ASSERT_NE((MockRobotManager*) NULL, mockRobotManager);
            ASSERT_NE((RobotManager*) NULL, robotManager);

            //-- Init image manager
            MockImageManager::RegisterManager();
            imageManager = ImageManager::getImageManager("MOCK");
            mockImageManager = dynamic_cast<MockImageManager *>(imageManager);
            ASSERT_NE((ImageManager*) NULL, imageManager);
            ASSERT_NE((MockImageManager*) NULL, mockImageManager);

            //-- Init mental map
            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*) NULL, mentalMap);
            ASSERT_TRUE(mentalMap->configure(1));

            std::vector<Player> players;
            players.push_back(Player(1,"test_player",MAX_HEALTH,MAX_HEALTH,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(Weapon("Default gun", 10, MAX_AMMO));

            //-- Init network manager
            MockNetworkManager::RegisterManager();
            networkManager = NetworkManager::getNetworkManager("MOCK");
            mockNetworkManager = dynamic_cast<MockNetworkManager *>(networkManager);
            ASSERT_NE((NetworkManager*) NULL, networkManager);
            ASSERT_NE((MockNetworkManager*) NULL, mockNetworkManager);
            networkManager->configure("player", players[0]);

            //-- Init screen manager
            SDLScreenManager::RegisterManager();
            screenManager = ScreenManager::getScreenManager("SDL");
            ASSERT_NE((ScreenManager*) NULL, screenManager);
            screenManager->start();

            //-- Setup Game FSM
            StateMachineBuilder builder;
            builder.setDirectorType("YARP");

            //-- Create states
            init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                               robotManager, audioManager, screenManager));
            game_state_id = builder.addState(new GameState(networkManager, imageManager, inputManager, mentalMap,
                                                               robotManager, audioManager, screenManager));
            dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                               mentalMap, robotManager, audioManager, screenManager));
            end_state_id = builder.addState(State::getEndState());

            //-- Add transitions to other states
            builder.addTransition(init_state_id, game_state_id, InitState::LOGIN_SUCCESSFUL);
            builder.addTransition(init_state_id, end_state_id, InitState::EXIT_REQUESTED);
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
            InputManager::destroyInputManager();
            inputManager = NULL;

            NetworkManager::destroyNetworkManager();
            networkManager = NULL;

            //-- Closing audio system:
            AudioManager::destroyAudioManager();
            audioManager = NULL;

            //-- Closing mental map:
            MentalMap::destroyMentalMap();
            mentalMap = NULL;

            //-- Close img related ports:
            ImageManager::destroyImageManager();
            imageManager = NULL;

            //-- Close robot:
            delete robotManager;
            robotManager = NULL;

            //-- Close screen:
            screenManager->stop();
            ScreenManager::destroyScreenManager();

            //-- Delete FSM:
            delete fsm;
            fsm = NULL;

            //-- Close YARP network
            yarp::os::Network::fini();
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

        int init_state_id, game_state_id, dead_state_id, end_state_id;

        Image test_frame_no_target;
        Image test_frame_with_target;
        static const std::string FRAME_NO_TARGET_PATH;
        static const std::string FRAME_WITH_TARGET_PATH;
};

const int RobotDevastationTest::MAX_HEALTH = 100;
const int RobotDevastationTest::MAX_AMMO = 10;
const std::string RobotDevastationTest::FRAME_NO_TARGET_PATH = "../images/test_frame_qr.ppm";
const std::string RobotDevastationTest::FRAME_WITH_TARGET_PATH = "../images/test_frame_qr_centered.ppm";


//--- Tests ------------------------------------------------------------------------------------------
TEST_F(RobotDevastationTest, RobotDevastationWorks)
{
    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_TRUE(mockInputManager->isStopped());
    ASSERT_FALSE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before login (loop):

    //yarp::os::Time::delay(1);
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_FALSE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- When enter is pressed, the system should log in and go to next state:
    mockInputManager->sendKeyPress(Key::KEY_ENTER);
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

    //-- If I send move commands, robot moves

    //-- Left
    mockInputManager->sendKeyDown(GameState::KEY_TURN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::LEFT, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TURN_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Right
    mockInputManager->sendKeyDown(GameState::KEY_TURN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::RIGHT,((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_TURN_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Forward
    mockInputManager->sendKeyDown(GameState::KEY_MOVE_FWD);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::FORWARD, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_MOVE_FWD);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

    //-- Backwards
    mockInputManager->sendKeyDown(GameState::KEY_MOVE_BACK);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::BACKWARDS, ((MockRobotManager *)robotManager)->getMovementDirection());
    mockInputManager->sendKeyUp(GameState::KEY_MOVE_BACK);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockRobotManager->isMoving());
    ASSERT_EQ(MockRobotManager::NONE, ((MockRobotManager *)robotManager)->getMovementDirection());

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
    for(int i = 0; i < MAX_AMMO; i++)
        mockInputManager->sendKeyPress(GameState::KEY_SHOOT);
    ASSERT_EQ(0, mentalMap->getCurrentWeapon().getCurrentAmmo());
    yarp::os::Time::delay(0.5);

    //-- After reloading, I can shoot again
    mockInputManager->sendKeyPress('r');
    ASSERT_EQ(MAX_AMMO, mentalMap->getCurrentWeapon().getCurrentAmmo());

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

    //-- Check that deadState is active
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());


    //-- Check things that should happen in dead state before time runs out (setup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When enter is pressed, but the countdown is still active, input is ignored
    yarp::os::Time::delay(0.5);
    mockInputManager->sendKeyPress(Key::KEY_ENTER);
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When time is up, and esc is pressed, the system should exit the game:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    mockInputManager->sendKeyPress(Key::KEY_ESCAPE);
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_TRUE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_DEAD"));
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
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RobotDevastationTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

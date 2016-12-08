#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/sig/all.h>
#include <yarp/os/ResourceFinder.h>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"
#include "SDLUtils.hpp"

#include "MockupNetworkManager.hpp"
#include "RdMockupImageManager.hpp"
#include "MockupInputManager.hpp"
#include "RdMentalMap.hpp"
#include "RdMockupRobotManager.hpp"
#include "MockupAudioManager.hpp"
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
            mockupAudioManager->load("RD_DEAD","RD_DEAD", AudioManager::MUSIC);
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
            players.push_back(RdPlayer(1,"test_player",MAX_HEALTH,MAX_HEALTH,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(RdWeapon("Default gun", 10, MAX_AMMO));

            //-- Init network manager
            MockupNetworkManager::RegisterManager();
            networkManager = RdNetworkManager::getNetworkManager("MOCKUP");
            mockupNetworkManager = dynamic_cast<MockupNetworkManager *>(networkManager);
            ASSERT_NE((RdNetworkManager*) NULL, networkManager);
            ASSERT_NE((MockupNetworkManager*) NULL, mockupNetworkManager);
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

        ScreenManager * screenManager;

        int init_state_id, game_state_id, dead_state_id, end_state_id;

        RdImage test_frame_no_target;
        RdImage test_frame_with_target;
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
    ASSERT_FALSE(mockupImageManager->isEnabled());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

    //-- When enter is pressed, the system should log in and go to next state:
    mockupInputManager->sendKeyPress(RdKey::KEY_ENTER);
    yarp::os::Time::delay(0.5);

    //-- Check things that should happen just after the fsm starts (after setup)
    //----------------------------------------------------------------------------
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupImageManager->isEnabled());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    ASSERT_TRUE(mockupRobotManager->isConnected());
    ASSERT_TRUE(mockupRobotManager->isEnabled());

    //-- Testing game flow
    //-----------------------------------------------------------------------------
    //-- Check that GameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

    //-- If my robot is hit, health decreases
    ASSERT_TRUE(mockupNetworkManager->sendPlayerHit(mentalMap->getMyself(), 50));
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(50, mentalMap->getMyself().getHealth());

    //-- If I send move commands, robot moves
    //-- Left
    mockupInputManager->sendKeyDown(RdKey::KEY_ARROW_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::LEFT, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    mockupInputManager->sendKeyUp(RdKey::KEY_ARROW_LEFT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::NONE, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    //-- Right
    mockupInputManager->sendKeyDown(RdKey::KEY_ARROW_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::RIGHT,((RdMockupRobotManager *)robotManager)->getMovementDirection());
    mockupInputManager->sendKeyUp(RdKey::KEY_ARROW_RIGHT);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::NONE, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    //-- Forward
    mockupInputManager->sendKeyDown(RdKey::KEY_ARROW_UP);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::FORWARD, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    mockupInputManager->sendKeyUp(RdKey::KEY_ARROW_UP);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::NONE, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    //-- Backwards
    mockupInputManager->sendKeyDown(RdKey::KEY_ARROW_DOWN);
    yarp::os::Time::delay(0.5);
    ASSERT_TRUE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::BACKWARDS, ((RdMockupRobotManager *)robotManager)->getMovementDirection());
    mockupInputManager->sendKeyUp(RdKey::KEY_ARROW_DOWN);
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(mockupRobotManager->isMoving());
    ASSERT_EQ(RdMockupRobotManager::NONE, ((RdMockupRobotManager *)robotManager)->getMovementDirection());

    //-- If I shoot with no target in the scope, the enemies life is kept equal
    mockupImageManager->receiveImage(test_frame_no_target);
    yarp::os::Time::delay(0.5);
    std::vector<RdPlayer> players_before = mentalMap->getPlayers();
    mockupInputManager->sendKeyPress(RdKey::KEY_SPACE);
    std::vector<RdPlayer> players_after = mentalMap->getPlayers();
    ASSERT_EQ(players_before.size(), players_after.size());
    for(int i = 0; i < players_before.size(); i++)
        EXPECT_EQ(players_before[i].getHealth(), players_after[i].getHealth());

    //-- If I shoot all ammo, I run out of ammo, and I cannot shoot until reloading
    for(int i = 0; i < MAX_AMMO; i++)
        mockupInputManager->sendKeyPress(RdKey::KEY_SPACE);
    ASSERT_EQ(0, mentalMap->getCurrentWeapon().getCurrentAmmo());
    yarp::os::Time::delay(0.5);

    //-- After reloading, I can shoot again
    mockupInputManager->sendKeyPress('r');
    ASSERT_EQ(MAX_AMMO, mentalMap->getCurrentWeapon().getCurrentAmmo());

    //-- If I hit other robot, other robot health decreases
    mockupImageManager->receiveImage(test_frame_with_target);
    yarp::os::Time::delay(0.5);
    players_before = mentalMap->getPlayers();
    mockupInputManager->sendKeyPress(RdKey::KEY_SPACE);
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

    //-- Check that deadState is active
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());


    //-- Check things that should happen in dead state before time runs out (setup):
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupImageManager->isEnabled());
    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_TRUE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

    //-- Check that deadState is active
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When enter is pressed, but the countdown is still active, input is ignored
    yarp::os::Time::delay(0.5);
    mockupInputManager->sendKeyPress(RdKey::KEY_ENTER);
    yarp::os::Time::delay(0.5);
    ASSERT_EQ(dead_state_id, fsm->getCurrentState());

    //-- When time is up, and esc is pressed, the system should exit the game:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockupInputManager->getNumListeners());
    mockupInputManager->sendKeyPress(RdKey::KEY_ESCAPE);
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupImageManager->isEnabled());
    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_DEAD"));
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_FALSE(mockupNetworkManager->isLoggedIn());
    ASSERT_FALSE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

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

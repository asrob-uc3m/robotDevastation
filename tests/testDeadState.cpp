// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testDeadState
 * ------------------------
 * Test that DeadState works correctly
 *
 * Note: to setup the initial conditions for the DeadState, the InitState is used in the test.
 * Therefore, please check InitState test if any error related to the initial conditions is found.
 *
 */

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include <yarp/sig/all.h>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "SDLUtils.hpp"
#include "DeadState.hpp"
#include "InitState.hpp"
#include "MockState.hpp"

#include "MockNetworkManager.hpp"
#include "MockImageManager.hpp"
#include "MockInputEventListener.hpp"
#include "MockInputManager.hpp"
#include "MentalMap.hpp"
#include "MockRobotManager.hpp"
#include "MockAudioManager.hpp"
#include "SDLScreenManager.hpp"

namespace rd
{

namespace test
{

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This is required since MockStates are used (and require yarp ports to be open)

class DeadStateTestEnvironment : public testing::Environment
{
    public:
        DeadStateTestEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
            screenManager = NULL;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();

            //-- Init SDL
            SDLScreenManager::RegisterManager();
            screenManager = ScreenManager::getScreenManager("SDL");
            ASSERT_NE((ScreenManager*) NULL, screenManager);
            screenManager->start();

        }

        virtual void TearDown()
        {
            yarp::os::Network::fini();

            screenManager->stop();
            ScreenManager::destroyScreenManager();
        }

    private:
        int argc;
        char ** argv;

    protected:
        ScreenManager * screenManager;
};

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class DeadStateTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Configure the ResourceFinder to obtain real paths to the resources
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

            //-- Create managers
            networkManager = NetworkManager::getNetworkManager("MOCK");
            mockNetworkManager = dynamic_cast<MockNetworkManager *>(networkManager);
            ASSERT_NE((NetworkManager*) NULL, networkManager);
            ASSERT_NE((MockNetworkManager*) NULL, mockNetworkManager);

            imageManager = ImageManager::getImageManager("MOCK");
            mockImageManager = dynamic_cast<MockImageManager *>(imageManager);
            ASSERT_NE((ImageManager*) NULL, imageManager);
            ASSERT_NE((MockImageManager*) NULL, mockImageManager);
            //-- Load test image
            Image test_frame;
            yarp::sig::file::read(test_frame, rf.findFileByName("../images/test_frame_qr.ppm"));
            mockImageManager->receiveImage(test_frame);

            inputManager = InputManager::getInputManager("MOCK");
            mockInputManager = dynamic_cast<MockInputManager *>(inputManager);
            ASSERT_NE((InputManager*) NULL, inputManager);
            ASSERT_NE((MockInputManager*) NULL, mockInputManager);

            audioManager = AudioManager::getAudioManager("MOCK");
            mockAudioManager = dynamic_cast<MockAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockAudioManager*) NULL, mockAudioManager);
            mockAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);
            mockAudioManager->load("RD_DEAD","RD_DEAD", AudioManager::MUSIC);

            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*) NULL, mentalMap);
            ASSERT_TRUE(mentalMap->configure(1));

            //-- Insert dead player for testing
            std::vector<Player> players;
            players.push_back(Player(1,"test_player", 0,MAX_HEALTH,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(Weapon("Default gun", 10, 5));
            networkManager->configure("player", players[0]);

            mockRobotManager = new MockRobotManager("MOCK");
            robotManager = (asrob::IRobotManager *) mockRobotManager;
            ASSERT_NE((MockRobotManager*) NULL, mockRobotManager);
            ASSERT_NE((asrob::IRobotManager*) NULL, robotManager);

            screenManager = ScreenManager::getScreenManager("SDL");
            ASSERT_NE((ScreenManager*) NULL, screenManager);

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

            //-- Finish setup of the modules that start at game state:
            //-- Note: we can change this to use GameState (as previously) if we want someday
            //-- Note2: I'm adding here a counter of the times I had problems due to not changing this.
            //-- If it were to arrive at 2 or 3, please change this.
            //-- Counter = 1
            mockImageManager->start();
            listener = new MockInputEventListener;
            mockInputManager->addInputEventListener(listener);
            audioManager->start();
            audioManager->play("RD_THEME", -1);
            mockRobotManager->setEnabled(true);
            mockImageManager->setEnabled(true);

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

            delete listener;
            listener=NULL;

        }

    static const int MAX_HEALTH;

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
        asrob::IRobotManager * robotManager;

        ScreenManager * screenManager;

        MockInputEventListener * listener;
};

const int DeadStateTest::MAX_HEALTH = 100;

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(DeadStateTest, DeadStateGoesToRespawn)
{
    //-- Create fsm with DeadState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager, screenManager));
    ASSERT_NE(-1, dead_state_id);
    int game_state_id = builder.addState(new MockState(1));
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
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

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

    //-- When time is up, and enter is pressed, the system should go to respawn state:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    mockInputManager->sendKeyPress(Key::KEY_ENTER);
    yarp::os::Time::delay(0.5);

    //-- Check that it has restored things (health, enable stuff)
    //-- and it is in the game state (cleanup):
    ASSERT_EQ(DeadStateTest::MAX_HEALTH, mentalMap->getMyself().getHealth());
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners()); //-- Game sets its own listener
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_FALSE(mockRobotManager->isEnabled());

    //-- Check that gameState is active
    ASSERT_EQ(game_state_id, fsm->getCurrentState());

}

TEST_F(DeadStateTest, DeadStateGoesToLogoutKeyPress)
{
    //-- Create fsm with DeadState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager, screenManager));
    ASSERT_NE(-1, dead_state_id);
    int game_state_id = builder.addState(new MockState(1));
    ASSERT_NE(-1, game_state_id);
    int exit_state_id = builder.addState(State::getEndState());
    ASSERT_NE(-1, exit_state_id);

    ASSERT_TRUE(builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED));
    ASSERT_TRUE(builder.addTransition(dead_state_id, exit_state_id, DeadState::EXIT_SELECTED));
    ASSERT_TRUE(builder.setInitialState(dead_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    // Player is dead
    // Stuff is enabled
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

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

TEST_F(DeadStateTest, DeadStateGoesToLogoutWindowEvent)
{
    //-- Create fsm with DeadState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int dead_state_id = builder.addState(new DeadState(networkManager, imageManager, inputManager,
                                                       mentalMap, robotManager, audioManager, screenManager));
    ASSERT_NE(-1, dead_state_id);
    int game_state_id = builder.addState(new MockState(1));
    ASSERT_NE(-1, game_state_id);
    int exit_state_id = builder.addState(State::getEndState());
    ASSERT_NE(-1, exit_state_id);

    ASSERT_TRUE(builder.addTransition(dead_state_id, game_state_id, DeadState::RESPAWN_SELECTED));
    ASSERT_TRUE(builder.addTransition(dead_state_id, exit_state_id, DeadState::EXIT_SELECTED));
    ASSERT_TRUE(builder.setInitialState(dead_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    // Player is dead
    // Stuff is enabled
    ASSERT_EQ(0, mentalMap->getMyself().getHealth()); //-- Important thing to check
    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_TRUE(mockImageManager->isEnabled());
    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_DEAD"));
    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());
    ASSERT_TRUE(mockRobotManager->isConnected());
    ASSERT_TRUE(mockRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

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

    //-- When time is up, and the window is closed, the system should exit the game:
    yarp::os::Time::delay(10);
    ASSERT_EQ(1, mockInputManager->getNumListeners());
    mockInputManager->sendWindowEvent(WindowEvent::WINDOW_CLOSE);
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

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new rd::test::DeadStateTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

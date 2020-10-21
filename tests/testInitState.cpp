// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "SDLUtils.hpp"
#include "InitState.hpp"

#include "MockNetworkManager.hpp"
#include "MockImageManager.hpp"
#include "MockInputManager.hpp"
#include "MentalMap.hpp"
#include "MockRobotManager.hpp"
#include "MockAudioManager.hpp"
#include "SDLScreenManager.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

namespace rd
{

namespace test
{

class InitStateTestEnvironment : public testing::Environment
{
    public:
        InitStateTestEnvironment(int argc, char ** argv)
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
class InitStateTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
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

            inputManager = InputManager::getInputManager("MOCK");
            mockInputManager = dynamic_cast<MockInputManager *>(inputManager);
            ASSERT_NE((InputManager*) NULL, inputManager);
            ASSERT_NE((MockInputManager*) NULL, mockInputManager);

            audioManager = AudioManager::getAudioManager("MOCK");
            mockAudioManager = dynamic_cast<MockAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockAudioManager*) NULL, mockAudioManager);
            mockAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);

            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*) NULL, mentalMap);
            ASSERT_TRUE(mentalMap->configure(1));

            std::vector<Player> players;
            players.push_back(Player(1,"test_player",100,100,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(Weapon("Default gun", 10, 5));
            networkManager->configure("player", players[0]);

            mockRobotManager = new MockRobotManager("MOCK");
            robotManager = (asrob::IRobotManager *) mockRobotManager;
            ASSERT_NE((MockRobotManager*) NULL, mockRobotManager);
            ASSERT_NE((asrob::IRobotManager*) NULL, robotManager);

            screenManager = ScreenManager::getScreenManager("SDL");
            ASSERT_NE((ScreenManager*) NULL, screenManager);
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

            ScreenManager::destroyScreenManager();

        }

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
};

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(InitStateTest, InitStateGoesToLogin)
{
    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_NE(-1, init_state_id);
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::LOGIN_SUCCESSFUL));
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(init_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_TRUE(mockInputManager->isStopped());

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

    //-- When enter is pressed, the system should log in and go to next state:
    mockInputManager->sendKeyPress(Key::KEY_ENTER);
    yarp::os::Time::delay(0.5);

    //-- Check that it has logged in and it is in the next state (cleanup):
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_FALSE(mockAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isLoggedIn());

    ASSERT_FALSE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());

    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(0, mockInputManager->getNumListeners());

}

TEST_F(InitStateTest, InitStateGoesToExitKeyPress)
{
    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_NE(-1, init_state_id);
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::LOGIN_SUCCESSFUL));
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(init_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_TRUE(mockInputManager->isStopped());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before exit (loop):

    //yarp::os::Time::delay(1);
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_FALSE(mockNetworkManager->isLoggedIn());

    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());

    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());

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

    //-- Check that end state is active
    ASSERT_EQ(-1, fsm->getCurrentState()); //-- (When FSM is ended, no state is active, hence -1)

}

TEST_F(InitStateTest, InitStateGoesToExitWindowEvent)
{
    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager, screenManager));
    int end_state_id = builder.addState(State::getEndState());

    ASSERT_NE(-1, init_state_id);
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::LOGIN_SUCCESSFUL));
    ASSERT_TRUE(builder.addTransition(init_state_id, end_state_id, InitState::EXIT_REQUESTED));
    ASSERT_TRUE(builder.setInitialState(init_state_id));

    fsm = builder.buildStateMachine();
    ASSERT_NE((FiniteStateMachine*)NULL, fsm);

    //-- Check things that should happen before fsm starts (before setup):
    ASSERT_TRUE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockNetworkManager->isStopped());
    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_TRUE(mockInputManager->isStopped());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before exit (loop):

    //yarp::os::Time::delay(1);
    ASSERT_FALSE(mockAudioManager->isStopped());
    ASSERT_TRUE(mockAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockNetworkManager->isStopped());
    ASSERT_FALSE(mockNetworkManager->isLoggedIn());

    ASSERT_TRUE(mockImageManager->isStopped());
    ASSERT_FALSE(mockImageManager->isEnabled());

    ASSERT_FALSE(mockInputManager->isStopped());
    ASSERT_EQ(1, mockInputManager->getNumListeners());

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

    //-- Check that end state is active
    ASSERT_EQ(-1, fsm->getCurrentState()); //-- (When FSM is ended, no state is active, hence -1)

}

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new rd::test::InitStateTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

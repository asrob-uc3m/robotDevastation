#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "Utils.hpp"
#include "SDLUtils.hpp"
#include "InitState.hpp"

#include "MockupNetworkManager.hpp"
#include "MockupImageManager.hpp"
#include "MockupInputManager.hpp"
#include "MentalMap.hpp"
#include "MockupRobotManager.hpp"
#include "MockupAudioManager.hpp"
#include "SDLScreenManager.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

using namespace rd;


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
            MockupNetworkManager::RegisterManager();
            MockupImageManager::RegisterManager();
            MockupInputManager::RegisterManager();
            MockupAudioManager::RegisterManager();
            SDLScreenManager::RegisterManager();

            //-- Create managers
            networkManager = NetworkManager::getNetworkManager("MOCKUP");
            mockupNetworkManager = dynamic_cast<MockupNetworkManager *>(networkManager);
            ASSERT_NE((NetworkManager*) NULL, networkManager);
            ASSERT_NE((MockupNetworkManager*) NULL, mockupNetworkManager);

            imageManager = ImageManager::getImageManager("MOCKUP");
            mockupImageManager = dynamic_cast<MockupImageManager *>(imageManager);
            ASSERT_NE((ImageManager*) NULL, imageManager);
            ASSERT_NE((MockupImageManager*) NULL, mockupImageManager);

            inputManager = InputManager::getInputManager("MOCKUP");
            mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);
            ASSERT_NE((InputManager*) NULL, inputManager);
            ASSERT_NE((MockupInputManager*) NULL, mockupInputManager);

            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);
            ASSERT_NE((AudioManager*) NULL, audioManager);
            ASSERT_NE((MockupAudioManager*) NULL, mockupAudioManager);
            mockupAudioManager->load("RD_THEME","RD_THEME", AudioManager::MUSIC);

            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*) NULL, mentalMap);
            ASSERT_TRUE(mentalMap->configure(1));

            std::vector<Player> players;
            players.push_back(Player(1,"test_player",100,100,0,0) );
            ASSERT_TRUE(mentalMap->updatePlayers(players));
            mentalMap->addWeapon(Weapon("Default gun", 10, 5));
            networkManager->configure("player", players[0]);

            mockupRobotManager = new MockupRobotManager("MOCKUP");
            robotManager = (RobotManager *) mockupRobotManager;
            ASSERT_NE((MockupRobotManager*) NULL, mockupRobotManager);
            ASSERT_NE((RobotManager*) NULL, robotManager);

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
            mockupNetworkManager = NULL;
            ImageManager::destroyImageManager();
            imageManager = NULL;
            mockupImageManager = NULL;
            InputManager::destroyInputManager();
            AudioManager::destroyAudioManager();

            MentalMap::destroyMentalMap();

            delete mockupRobotManager;
            mockupRobotManager = NULL;

            ScreenManager::destroyScreenManager();

        }

    protected:
        FiniteStateMachine *fsm;

        NetworkManager * networkManager;
        MockupNetworkManager * mockupNetworkManager;

        ImageManager * imageManager;
        MockupImageManager * mockupImageManager;

        InputManager * inputManager;
        MockupInputManager * mockupInputManager;

        AudioManager * audioManager;
        MockupAudioManager * mockupAudioManager;

        MentalMap * mentalMap;

        MockupRobotManager * mockupRobotManager;
        RobotManager * robotManager;

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
    mockupInputManager->sendKeyPress(Key::KEY_ENTER);
    yarp::os::Time::delay(0.5);

    //-- Check that it has logged in and it is in the next state (cleanup):
    ASSERT_FALSE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));

    ASSERT_FALSE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isLoggedIn());

    ASSERT_FALSE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupImageManager->isEnabled());

    ASSERT_FALSE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());

    ASSERT_TRUE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

}

TEST_F(InitStateTest, InitStateGoesToExit)
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
    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_TRUE(mockupNetworkManager->isStopped());
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_FALSE(mockupRobotManager->isConnected());
    ASSERT_FALSE(mockupRobotManager->isEnabled());

    //-- Start state machine
    ASSERT_TRUE(fsm->start());

    //-- Check things that should happen in initial state before exit (loop):

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

    //-- When esc is pressed, the system should exit the game:
    mockupInputManager->sendKeyPress(Key::KEY_ESCAPE);
    yarp::os::Time::delay(0.5);

    //-- Check that it has stopped things and it is in the final state (cleanup):
    ASSERT_TRUE(mockupImageManager->isStopped());
    ASSERT_FALSE(mockupImageManager->isEnabled());

    ASSERT_TRUE(mockupInputManager->isStopped());
    ASSERT_EQ(0, mockupInputManager->getNumListeners());

    ASSERT_TRUE(mockupAudioManager->isStopped());
    ASSERT_FALSE(mockupAudioManager->isPlaying("RD_THEME"));

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
  testing::Environment* env = testing::AddGlobalTestEnvironment(new InitStateTestEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

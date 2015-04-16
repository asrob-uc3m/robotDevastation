#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "StateMachine.hpp"
#include "StateMachineBuilder.hpp"
#include "RdUtils.hpp"
#include "InitState.hpp"

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
class InitStateTest : public testing::Test
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

            imageManager = RdImageManager::getImageManager("MOCKUP");
            mockupImageManager = dynamic_cast<RdMockupImageManager *>(imageManager);

            inputManager = RdInputManager::getInputManager("MOCKUP");
            mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);

            audioManager = AudioManager::getAudioManager("MOCKUP");
            mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);

            mentalMap = RdMentalMap::getMentalMap();

            mockupRobotManager = new RdMockupRobotManager(0);
            robotManager = (RdRobotManager *) mockupRobotManager;

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

        RdMentalMap * mentalMap = RdMentalMap::getMentalMap();

        RdMockupRobotManager * mockupRobotManager;
        RdRobotManager * robotManager;
};

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(InitStateTest, InitStateWorksCorrectly )
{


    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState(networkManager, imageManager, inputManager, mentalMap,
                                                       robotManager, audioManager));

}

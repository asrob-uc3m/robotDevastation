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

        }

        virtual void TearDown()
        {
            //-- Close YARP network
            yarp::os::Network::fini();
        }

    protected:
        FiniteStateMachine *fsm;
};

//--- Tests ------------------------------------------------------------------------------------------
TEST_F(InitStateTest, InitStateWorksCorrectly )
{
    //-- Register managers to be used:
    MockupNetworkManager::RegisterManager();
    RdMockupImageManager::RegisterManager();
    MockupInputManager::RegisterManager();
    MockupAudioManager::RegisterManager();

    //-- Create managers
    RdNetworkManager * networkManager = RdNetworkManager::getNetworkManager("MOCKUP");
    MockupNetworkManager * mockupNetworkManager = dynamic_cast<MockupNetworkManager *>(networkManager);

    RdImageManager * imageManager = RdImageManager::getImageManager("MOCKUP");
    RdMockupImageManager * mockupImageManager = dynamic_cast<RdMockupImageManager *>(imageManager);

    RdInputManager * inputManager = RdInputManager::getInputManager("MOCKUP");
    MockupInputManager * mockupInputManager = dynamic_cast<MockupInputManager *>(inputManager);

    AudioManager * audioManager = AudioManager::getAudioManager("MOCKUP");
    MockupAudioManager * mockupAudioManager = dynamic_cast<MockupAudioManager *>(audioManager);

    RdMentalMap * mentalMap = RdMentalMap::getMentalMap();

    RdMockupRobotManager * mockupRobotManager = new RdMockupRobotManager(0);

    //-- Create fsm with InitState
    StateMachineBuilder builder;
    ASSERT_TRUE(builder.setDirectorType("YARP"));

    int init_state_id = builder.addState(new InitState());

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

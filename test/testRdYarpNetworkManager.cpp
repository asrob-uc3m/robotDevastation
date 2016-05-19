/***
 * testRdYarpNetworkManager
 *
 * Test for testing the client interface with the server
 *
 * IMPORTANT: this test requires an instance of the rdServer to be up and running
 *
 ***/

#include <yarp/os/Thread.h>

#include "RdYarpNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"
#include "RdServer.hpp"

#include "gtest/gtest.h"

using namespace rd;

class RunningRdServerThread: public yarp::os::Thread
{
    public:
        RunningRdServerThread(int argc, char** argv) {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void run() {
            rdServer.runModule(argc, argv);
        }

        virtual void onStop() {
            rdServer.stopModule();
        }

    private:
        rd::RdServer rdServer;
        int argc;
        char** argv;
};

class RdYarpNetworkManagerTest : public testing::Test
{
    public:    
        RdYarpNetworkManagerTest(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            ASSERT_TRUE(RdYarpNetworkManager::RegisterManager());
            networkManager = RdYarpNetworkManager::getNetworkManager();
            ASSERT_TRUE(networkManager);

            me = new RdPlayer(0, "Myself", 100, 100, 0, 0);

            rdServer = new RunningRdServerThread(argc, argv);
            rdServer->run();
        }

        virtual void TearDown()
        {
            rdserver->stop();
            delete rdServer;
            rdServer = NULL;

            ASSERT_TRUE(RdYarpNetworkManager::destroyNetworkManager());
            delete me;
            me = NULL;
        }

        RdPlayer * me;

    protected:
        RdNetworkManager * networkManager;
        RunningRdServerThread * rdserver;

    private:
        int argc;
        char ** argv;
};


//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
class RdYarpNetworkManagerEnvironment : public testing::Environment
{
    public:
        RdYarpNetworkManagerEnvironment(int argc, char ** argv)
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


TEST_F( RdYarpNetworkManagerTest, NetworkManagerIsSingleton)
{
    RdNetworkManager * networkManager2 = NULL;
    networkManager2 = RdYarpNetworkManager::getNetworkManager();

    ASSERT_NE((RdNetworkManager *)NULL, networkManager);
    ASSERT_NE((RdNetworkManager *)NULL, networkManager2);
    ASSERT_EQ(networkManager, networkManager2);
}

TEST_F( RdYarpNetworkManagerTest, NetworkManagerLoginLogout)
{
    ASSERT_TRUE(networkManager->login(*me));
    ASSERT_TRUE(networkManager->logout(*me));
}

TEST_F( RdYarpNetworkManagerTest, NetworkManagerSendHit)
{
    ASSERT_TRUE(networkManager->login(*me));
    ASSERT_TRUE(networkManager->sendPlayerHit(*me, 50));
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RdYarpNetworkManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

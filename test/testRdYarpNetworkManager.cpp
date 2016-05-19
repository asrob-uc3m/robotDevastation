/***
 * testRdYarpNetworkManager
 *
 * Test for testing the client interface with the server
 *
 * IMPORTANT: this test requires an instance of the rdServer to be up and running
 *
 ***/

#include <yarp/os/Thread.h>
#include <yarp/os/ResourceFinder.h>

#include "RdYarpNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"
#include "RdServer.hpp"

#include "gtest/gtest.h"

using namespace rd;

class RunningRdServerThread: public yarp::os::Thread
{
    public:
        RunningRdServerThread(yarp::os::ResourceFinder& rf) {
            this->rf = rf;
        }

        virtual void run() {
            rdServer.runModule(rf);
            RD_DEBUG("Run Module!\n");
        }

        virtual void onStop() {
            RD_DEBUG("Request Module stop\n");
            rdServer.stopModule(true);
            RD_DEBUG("Module stopped\n");
        }

        virtual ~RunningRdServerThread() {}

    private:
        yarp::os::ResourceFinder rf;
        rd::RdServer rdServer;
        int argc;
        char** argv;
};

class RdYarpNetworkManagerTest : public testing::Test
{
    public:    
        virtual void SetUp()
        {
            ASSERT_TRUE(RdYarpNetworkManager::RegisterManager());
            networkManager = RdYarpNetworkManager::getNetworkManager();
            ASSERT_TRUE(networkManager);

            me = new RdPlayer(0, "Myself", 100, 100, 0, 0);

            RD_DEBUG("Running rdServer\n");
            yarp::os::ResourceFinder rf;
            rdServer = new RunningRdServerThread(rf);
            rdServer->start();
            yarp::os::Time::delay(1);
            RD_DEBUG("rdServer now running\n");
        }

        virtual void TearDown()
        {
            RD_DEBUG("Stopping rdServer\n");
            rdServer->stop();
            delete rdServer;
            rdServer = NULL;

            ASSERT_TRUE(RdYarpNetworkManager::destroyNetworkManager());
            delete me;
            me = NULL;
        }

        RdPlayer * me;

    protected:
        RdNetworkManager * networkManager;
        RunningRdServerThread * rdServer;

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
};


TEST_F( RdYarpNetworkManagerTest, NetworkManagerIsSingleton)
{
    RD_WARNING("TEST#1!\n");
    RdNetworkManager * networkManager2 = NULL;
    networkManager2 = RdYarpNetworkManager::getNetworkManager();

    ASSERT_NE((RdNetworkManager *)NULL, networkManager);
    ASSERT_NE((RdNetworkManager *)NULL, networkManager2);
    ASSERT_EQ(networkManager, networkManager2);
    RD_WARNING("TEST#1! - FINISHED\n");
}

TEST_F( RdYarpNetworkManagerTest, NetworkManagerLoginLogout)
{
    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->logout());
}

TEST_F( RdYarpNetworkManagerTest, NetworkManagerSendHit)
{
    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->sendPlayerHit(*me, 50));
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RdYarpNetworkManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

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

#include "YarpNetworkManager.hpp"
#include "MockupNetworkEventListener.hpp"
#include "Server.hpp"

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

            me = RdPlayer(0, "me", 100, 100, 0, 0);
            other_player = RdPlayer(1, "dummy", 100, 100, 1, 0);

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
        }

        RdPlayer me, other_player;

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

//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(RdYarpNetworkManagerTest, ManagerDoesNotStartIfNotConfigured)
{
    ASSERT_FALSE(networkManager->start());
    ASSERT_TRUE(networkManager->isStopped());

    networkManager->configure("player", me);

    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(RdYarpNetworkManagerTest, NetworkManagerIsSingleton)
{
    RdNetworkManager * networkManager2 = NULL;
    networkManager2 = RdYarpNetworkManager::getNetworkManager();

    ASSERT_NE((RdNetworkManager *)NULL, networkManager);
    ASSERT_NE((RdNetworkManager *)NULL, networkManager2);
    ASSERT_EQ(networkManager, networkManager2);
}

TEST_F(RdYarpNetworkManagerTest, NetworkManagerAPIWorks)
{
    MockupNetworkEventListener listener;
    RdNetworkEventListener * plistener = (RdNetworkEventListener *) &listener;
    ASSERT_TRUE(((RdNetworkManager*)networkManager)->addNetworkEventListener(plistener));

    //-- Startup
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    //-- Login
    ASSERT_TRUE(networkManager->login());
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(networkManager->login());//--Check that it is not allowed to login twice

    std::vector<RdPlayer> players = listener.getStoredPlayers();
    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());
    listener.resetDataArrived();

    //-- Damage player
    ASSERT_TRUE(networkManager->sendPlayerHit(me, 50));
    yarp::os::Time::delay(0.5);
    players = listener.getStoredPlayers();

    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());
    EXPECT_EQ(50, players[0].getHealth());
    listener.resetDataArrived();

    //-- Logout
    ASSERT_TRUE(networkManager->logout());
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(networkManager->logout());//--Check that it is not allowed to logout twice

    /* Check that data > 0 and check contents */
    players = listener.getStoredPlayers();

    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(0, players.size());
    listener.resetDataArrived();

    //-- Stop
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(RdYarpNetworkManagerTest, DisconnectedIfNoKeepAlive)
{
    MockupNetworkEventListener listener;
    RdNetworkEventListener * plistener = (RdNetworkEventListener *) &listener;
    ASSERT_TRUE(((RdNetworkManager*)networkManager)->addNetworkEventListener(plistener));

    //-- Startup
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    //-- Login
    ASSERT_TRUE(networkManager->login());
    yarp::os::Time::delay(0.5);

    std::vector<RdPlayer> players = listener.getStoredPlayers();
    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());
    listener.resetDataArrived();

    //-- Wait more than the timeout time
    yarp::os::Time::delay(60+1); //-- This should be really hardcoded, but it is the fastest implementation right now

    //-- Check that I'm no longer logged in
    players = listener.getStoredPlayers();
    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(0, players.size());
    listener.resetDataArrived();

    //-- Logout
    ASSERT_FALSE(networkManager->logout()); //-- If you try to log out when already logged out you get an error

    //-- Stop
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RdYarpNetworkManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testYarpNetworkManager
 *
 * Test for testing the client interface with the server
 *
 * IMPORTANT: this test requires an instance of the rdServer to be up and running
 *
 ***/

#include <yarp/os/Thread.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

#include "YarpNetworkManager.hpp"
#include "MockNetworkEventListener.hpp"
#include "Server.hpp"
#include "RpcResponder.hpp"
#include "Macros.hpp"

#include "gtest/gtest.h"

namespace rd
{
    class YarpNetworkManagerNoKeepAlive;
}

class rd::YarpNetworkManagerNoKeepAlive : public YarpNetworkManager
{
    public:
        static bool RegisterManager();

        static const std::string id;

    protected:
        void run()
        {
            // noop, overrides YarpNetworkManager::run
        }

    private:
        YarpNetworkManagerNoKeepAlive() {}

        static YarpNetworkManagerNoKeepAlive * uniqueInstance;
};

using namespace rd;

YarpNetworkManagerNoKeepAlive * YarpNetworkManagerNoKeepAlive::uniqueInstance = NULL;
const std::string YarpNetworkManagerNoKeepAlive::id = "YARP_NO_KEEP_ALIVE";

bool YarpNetworkManagerNoKeepAlive::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new YarpNetworkManagerNoKeepAlive();
    }

    return Register(uniqueInstance, id);
}

class YarpNetworkManagerTestBase : public testing::Test
{
    public:
        virtual void SetUp()
        {
            ASSERT_TRUE(registerNetworkManager());
            networkManager = NetworkManager::getNetworkManager();
            ASSERT_NE((NetworkManager *)NULL, networkManager);

            me = Player(0, "me", 100, 100, 0, 0);
            other_player = Player(1, "dummy", 100, 100, 1, 0);

            RD_DEBUG("Running rdServer\n");
            rf = new yarp::os::ResourceFinder();
            rf->setDefault("quiet", yarp::os::Value());
            rdServer.configure(*rf);
            rdServer.runModuleThreaded();
            RD_DEBUG("rdServer now running\n");
        }

        virtual void TearDown()
        {
            RD_DEBUG("Stopping rdServer\n");
            rdServer.stopModule(true);
            ASSERT_TRUE(NetworkManager::destroyNetworkManager());

            delete rf;
            rf = NULL;
        }

        Player me, other_player;

    protected:
        virtual bool registerNetworkManager() = 0;

        yarp::os::ResourceFinder * rf;
        NetworkManager * networkManager;
        Server rdServer;
};

class YarpNetworkManagerTest : public YarpNetworkManagerTestBase
{
    protected:
        virtual bool registerNetworkManager()
        {
            return YarpNetworkManager::RegisterManager();
        }
};

class YarpNetworkManagerNoKeepAliveTest : public YarpNetworkManagerTestBase
{
    protected:
        virtual bool registerNetworkManager()
        {
            return YarpNetworkManagerNoKeepAlive::RegisterManager();
        }
};


//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
class YarpNetworkManagerEnvironment : public testing::Environment
{
    public:
        YarpNetworkManagerEnvironment(int argc, char ** argv)
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
TEST_F(YarpNetworkManagerTest, ManagerDoesNotStartIfNotConfigured)
{
    ASSERT_FALSE(networkManager->start());
    ASSERT_TRUE(networkManager->isStopped());

    networkManager->configure("player", me);

    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(YarpNetworkManagerTest, NetworkManagerIsSingleton)
{
    NetworkManager * networkManager2 = NULL;
    networkManager2 = YarpNetworkManager::getNetworkManager();

    ASSERT_NE((NetworkManager *)NULL, networkManager);
    ASSERT_NE((NetworkManager *)NULL, networkManager2);
    ASSERT_EQ(networkManager, networkManager2);
}

TEST_F(YarpNetworkManagerTest, NetworkManagerAPIWorks)
{
    MockNetworkEventListener listener;
    NetworkEventListener * plistener = &listener;
    ASSERT_TRUE(networkManager->addNetworkEventListener(plistener));

    //-- Startup
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    //-- Login
    ASSERT_TRUE(networkManager->login());
    yarp::os::Time::delay(0.5);
    ASSERT_FALSE(networkManager->login());//--Check that it is not allowed to login twice

    std::vector<Player> players = listener.getStoredPlayers();
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

TEST_F(YarpNetworkManagerNoKeepAliveTest, DisconnectedIfNoKeepAlive)
{
    MockNetworkEventListener listener;
    NetworkEventListener * plistener = &listener;
    ASSERT_TRUE(networkManager->addNetworkEventListener(plistener));

    //-- Startup
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    //-- Login
    ASSERT_TRUE(networkManager->login());
    yarp::os::Time::delay(0.5);

    std::vector<Player> players = listener.getStoredPlayers();
    EXPECT_LE(1, listener.getDataArrived());
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());
    listener.resetDataArrived();

    //-- Wait more than the timeout time
    double delay = RpcResponder::MAX_BELIEF * rdServer.getPeriod();
    RD_INFO("Waiting %.1f + 1 seconds...\n", delay);
    yarp::os::Time::delay(delay + 1);

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
  testing::Environment* env = testing::AddGlobalTestEnvironment(new YarpNetworkManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}

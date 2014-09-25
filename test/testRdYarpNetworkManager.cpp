/***
 * testRdYarpNetworkManager
 *
 * Test for testing the client interface with the server
 *
 * IMPORTANT: this test requires an instance of the rdServer to be up and running
 *
 ***/

#include "RdYarpNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"

#include "gtest/gtest.h"

using namespace rd;

class RdYarpNetworkManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            networkManager = RdYarpNetworkManager::getNetworkManager();

            me = new RdPlayer(0, "Myself", 100, 100, 0, 0);
        }

        virtual void TearDown()
        {
            RdYarpNetworkManager::destroyNetworkManager();
            delete me;
            me = NULL;
        }

        RdPlayer * me;

    protected:
        RdNetworkManager * networkManager;
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



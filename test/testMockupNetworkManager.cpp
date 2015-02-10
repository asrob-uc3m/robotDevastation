/*
 * Cases:
 * If login then a player is created
 * If login twice returns an error
 * If logout player is removed
 * if logout twice returns an error
 * Check player upload with setPlayer
 * Check damaged player with sendPlayerHit
 * Check listener when data is sent <- (remaining case)
 */

#include "gtest/gtest.h"
#include "MockupNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"

using namespace rd;

/**
 * @brief Dummy RdNetworkEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received data and has a counter of
 * the incoming messages received
 *
 *  @todo Move this class to RdNetworkLib, set it to be compiled only with the tests
 */
class MockupNetworkEventListener : public RdNetworkEventListener
{
    public:
        MockupNetworkEventListener()
        {
            data_arrived = 0;
        }

        virtual bool onDataArrived(std::vector<RdPlayer> players)
        {
            stored_players = players;
            data_arrived++;
        }

        int getDataArrived() { return data_arrived; }
        void resetDataArrived() { data_arrived = 0; }

        std::vector<RdPlayer> getStoredPlayers() { return stored_players; }


    private:
        int data_arrived;
        std::vector<RdPlayer> stored_players;
};

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockupNetworkManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            MockupNetworkManager::RegisterManager();
            networkManager = (MockupNetworkManager *) RdNetworkManager::getNetworkManager(MockupNetworkManager::id);

            me = RdPlayer(0, "me", 100, 100, 0, 0);
            other_player = RdPlayer(1, "dummy", 100, 100, 1, 0);
        }

        virtual void TearDown()
        {
            RdNetworkManager::destroyNetworkManager();
        }


    protected:
        MockupNetworkManager * networkManager;
        RdPlayer me, other_player;
};

//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(MockupNetworkManagerTest, PlayerCreatedWhenLogin)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login(me));
    ASSERT_TRUE(networkManager->isLoggedIn());


    std::vector<RdPlayer> players = networkManager->getPlayerData();
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, ErrorLoginTwice)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login(me));
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<RdPlayer> players = networkManager->getPlayerData();
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_FALSE(networkManager->login(other_player));
    ASSERT_TRUE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, PlayerRemovedOnLogout)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login(me));
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<RdPlayer> players = networkManager->getPlayerData();
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->logout(me));
    EXPECT_FALSE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    EXPECT_EQ(0, players.size());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, ErrorLogoutTwice)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login(me));
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<RdPlayer> players = networkManager->getPlayerData();
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->logout(me));
    EXPECT_FALSE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    EXPECT_EQ(0, players.size());

    EXPECT_FALSE(networkManager->logout(me));
    EXPECT_FALSE(networkManager->isLoggedIn());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, SetPlayerAddsPlayer)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    std::vector<RdPlayer> players;
    players.push_back(me);
    players.push_back(other_player);

    EXPECT_TRUE(networkManager->setPlayerData(players));

    players = networkManager->getPlayerData();
    ASSERT_EQ(2, players.size());
    EXPECT_EQ(0, players[0].getId());
    EXPECT_EQ(1, players[1].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, PlayerDamagedWhenShot)
{
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    std::vector<RdPlayer> players;
    players.push_back(me);
    players.push_back(other_player);

    EXPECT_TRUE(networkManager->setPlayerData(players));

    players = networkManager->getPlayerData();
    ASSERT_EQ(2, players.size());

    ASSERT_TRUE(networkManager->sendPlayerHit(other_player, 100));

    players = networkManager->getPlayerData();
    ASSERT_EQ(2, players.size());
    EXPECT_EQ(0, players[1].getHealth());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockupNetworkManagerTest, ListenersNotifiedOnEvent)
{
    MockupNetworkEventListener listener;
    RdNetworkEventListener * plistener = (RdNetworkEventListener *) &listener;
    ASSERT_TRUE(((RdNetworkManager*)networkManager)->addNetworkEventListener(plistener));

    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login(me));
    ASSERT_TRUE(networkManager->isLoggedIn());

    /* Check that data > 0 and check contents */
    ASSERT_TRUE(networkManager->sendPlayerData());
    std::vector<RdPlayer> players = listener.getStoredPlayers();

    EXPECT_EQ(1, listener.getDataArrived());
    EXPECT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

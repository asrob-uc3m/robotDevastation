// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"
#include "MockNetworkManager.hpp"
#include "MockNetworkEventListener.hpp"
#include "MentalMap.hpp"

namespace rd
{

namespace test
{

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockNetworkManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            MockNetworkManager::RegisterManager();
            networkManager = (MockNetworkManager *) NetworkManager::getNetworkManager(MockNetworkManager::id);

            me = Player(0, "me", 100, 100, 0, 0);
            other_player = Player(1, "dummy", 100, 100, 1, 0);
        }

        virtual void TearDown()
        {
            NetworkManager::destroyNetworkManager();
        }


    protected:
        MockNetworkManager * networkManager;
        Player me, other_player;
};


//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(MockNetworkManagerTest, ManagerDoesNotStartIfNotConfigured)
{
    ASSERT_FALSE(networkManager->start());
    ASSERT_TRUE(networkManager->isStopped());

    networkManager->configure("player", me);

    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, PlayerCreatedWhenLogin)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());


    std::vector<Player> players = networkManager->getPlayerData();
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, ErrorLoginTwice)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<Player> players = networkManager->getPlayerData();
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_FALSE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, PlayerRemovedOnLogout)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<Player> players = networkManager->getPlayerData();
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->logout());
    EXPECT_FALSE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    ASSERT_EQ(0, players.size());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, ErrorLogoutTwice)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    std::vector<Player> players = networkManager->getPlayerData();
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->logout());
    EXPECT_FALSE(networkManager->isLoggedIn());

    players = networkManager->getPlayerData();
    ASSERT_EQ(0, players.size());

    EXPECT_FALSE(networkManager->logout());
    EXPECT_FALSE(networkManager->isLoggedIn());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, SetPlayerAddsPlayer)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    std::vector<Player> players;
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

TEST_F(MockNetworkManagerTest, PlayerDamagedWhenShot)
{
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    std::vector<Player> players;
    players.push_back(me);
    players.push_back(other_player);

    EXPECT_TRUE(networkManager->setPlayerData(players));
    ASSERT_TRUE(networkManager->setLoggedIn(true));

    players = networkManager->getPlayerData();
    ASSERT_EQ(2, players.size());

    ASSERT_TRUE(networkManager->sendPlayerHit(other_player, 100));

    players = networkManager->getPlayerData();
    ASSERT_EQ(2, players.size());
    EXPECT_EQ(0, players[1].getHealth());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, ListenersNotifiedOnEvent)
{
    MockNetworkEventListener listener;
    NetworkEventListener * plistener = (NetworkEventListener *) &listener;
    ASSERT_TRUE(((NetworkManager*)networkManager)->addNetworkEventListener(plistener));

    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    /* Check that data > 0 and check contents */
    std::vector<Player> players = listener.getStoredPlayers();

    EXPECT_EQ(1, listener.getDataArrived());
    ASSERT_EQ(1, players.size());
    EXPECT_EQ(0, players[0].getId());

    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
}

TEST_F(MockNetworkManagerTest, ManagerIsIntegratedWithMentalMap)
{
    //-------------------------------------------------------------------------------------
    //-- This tests integration between MockNetworkManager and MentalMap
    //-- More precisely, that targets are updated in MentalMap when shot, with information
    //-- going from MentalMap to NetworkManager and then back to MentalMap
    //-------------------------------------------------------------------------------------

    //-- Create a mental map with player & weapon info
    MentalMap * mentalMap = MentalMap::getMentalMap();
    ASSERT_NE((MentalMap*) NULL, mentalMap);
    mentalMap->addWeapon(Weapon("Machine gun", 10, 10));
    ASSERT_TRUE(mentalMap->configure(me.getId()));
    std::vector<Player> players;
    players.push_back(me);
    players.push_back(other_player);
    ASSERT_TRUE(mentalMap->updatePlayers(players));
    std::vector<Target> targets;
    targets.push_back(Target(other_player.getId(), Vector2d(Weapon::SCOPE_X, Weapon::SCOPE_Y),
                               Vector2d(50, 50)));
    ASSERT_TRUE(mentalMap->updateTargets(targets));

    //-- Attach mental map to networkManager
    mentalMap->addMentalMapEventListener(networkManager);

    //-- Start networkManager
    networkManager->configure("player", me);
    ASSERT_TRUE(networkManager->start());
    ASSERT_FALSE(networkManager->isStopped());

    ASSERT_TRUE(networkManager->login());
    ASSERT_TRUE(networkManager->isLoggedIn());

    //-- Push notification from mental map
    mentalMap->shoot();

    //-- Check everything worked as expected
    std::vector<Player> players_after = mentalMap->getPlayers();
    for (std::vector<Player>::iterator it = players_after.begin(); it != players_after.end(); ++it)
        if (it->getId() == 2)
            ASSERT_LT(it->getHealth(), 100);

    //-- Cleanup
    ASSERT_TRUE(networkManager->stop());
    ASSERT_TRUE(networkManager->isStopped());
    MentalMap::destroyMentalMap();
}

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

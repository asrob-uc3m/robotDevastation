#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "RdMentalMap.hpp"
#include "RdTarget.hpp"
#include "RdPlayer.hpp"
#include "SDLAudioManager.hpp"

using namespace rd;

class RdMentalMapTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            SDLAudioManager::RegisterManager();
            mentalMap = RdMentalMap::getMentalMap();

            player1 = new RdPlayer(0, "Myself", 100, 100, 0, 0);
            player2 = new RdPlayer(1, "Enemy", 100, 100, 1, 0);
            player3 = new RdPlayer(2, "Enemy2", 100, 100, 1, 0);

            target1 = new RdTarget(1, RdVector2d(100, 100), RdVector2d(50, 50));
            target2 = new RdTarget(2, RdVector2d( 20,  20), RdVector2d( 5,  5));
        }

        virtual void TearDown()
        {
            RdMentalMap::destroyMentalMap();
            SDLAudioManager::destroyAudioManager();

            delete player1; player1 = NULL;
            delete player2; player2 = NULL;
            delete player3; player3 = NULL;

            delete target1; target1 = NULL;
            delete target2; target2 = NULL;
        }

        RdPlayer * player1, * player2, * player3;

        RdTarget * target1, * target2;

    protected:
        RdMentalMap * mentalMap;
};

TEST_F( RdMentalMapTest, MentalMapIsSingleton)
{
    RdMentalMap * map2 = NULL;
    map2 = RdMentalMap::getMentalMap();

    ASSERT_NE((RdMentalMap *)NULL, mentalMap);
    ASSERT_NE((RdMentalMap *)NULL, map2);
    ASSERT_EQ(mentalMap, map2);
}

TEST_F( RdMentalMapTest, ConfigureMentalMap)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for two players:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Check the number of players
    std::vector<RdPlayer> players = mentalMap->getPlayers();
    EXPECT_EQ(0, (int) players.size());

    //-- Check the number of targets
    std::vector<RdTarget> targets = mentalMap->getTargets();
    EXPECT_EQ(0, (int) targets.size());
}



//---------------------------------------------------------------------------------------
//-- Player-related
//---------------------------------------------------------------------------------------

TEST_F( RdMentalMapTest, UpdatePlayersUpdatesPlayers)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for two players:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Update players:
    std::vector<RdPlayer> players;
    players.push_back(*player1);
    players.push_back(*player2);

    ASSERT_TRUE(mentalMap->updatePlayers(players));

    //-- Check players stored:
    std::vector<RdPlayer> players_stored = mentalMap->getPlayers();
    ASSERT_EQ(2, (int) players_stored.size());

    if (players_stored[0].getId() == 0)
    {
        EXPECT_STREQ("Myself", players_stored[0].getName().c_str());
        EXPECT_EQ(100, players_stored[0].getHealth());
        EXPECT_EQ(100, players_stored[0].getMaxHealth());
        EXPECT_EQ(0, players_stored[0].getTeamId());
        EXPECT_EQ(0, players_stored[0].getScore());

        EXPECT_EQ(1, players_stored[1].getId());
        EXPECT_STREQ("Enemy", players_stored[1].getName().c_str());
        EXPECT_EQ(100, players_stored[1].getHealth());
        EXPECT_EQ(100, players_stored[1].getMaxHealth());
        EXPECT_EQ(1, players_stored[1].getTeamId());
        EXPECT_EQ(0, players_stored[1].getScore());
    }
    else if (players_stored[1].getId() == 0)
    {
        EXPECT_STREQ("Myself", players_stored[1].getName().c_str());
        EXPECT_EQ(100, players_stored[1].getHealth());
        EXPECT_EQ(100, players_stored[1].getMaxHealth());
        EXPECT_EQ(0, players_stored[1].getTeamId());
        EXPECT_EQ(0, players_stored[1].getScore());

        EXPECT_EQ(1, players_stored[0].getId());
        EXPECT_STREQ("Enemy", players_stored[0].getName().c_str());
        EXPECT_EQ(100, players_stored[0].getHealth());
        EXPECT_EQ(100, players_stored[0].getMaxHealth());
        EXPECT_EQ(1, players_stored[0].getTeamId());
        EXPECT_EQ(0, players_stored[0].getScore());
    }
    else
    {
        //-- Break test
        ASSERT_TRUE(false);
    }


    //-- Change players & update mental map:
    players[0].setHealth(50);
    players[1].setScore(100);
    ASSERT_TRUE(mentalMap->updatePlayers(players));


    //-- Check players stored:
    players_stored = mentalMap->getPlayers();
    ASSERT_EQ(2, (int) players_stored.size());

    if (players_stored[0].getId() == 0)
    {
        EXPECT_STREQ("Myself", players_stored[0].getName().c_str());
        EXPECT_EQ(50, players_stored[0].getHealth());
        EXPECT_EQ(100, players_stored[0].getMaxHealth());
        EXPECT_EQ(0, players_stored[0].getTeamId());
        EXPECT_EQ(0, players_stored[0].getScore());

        EXPECT_EQ(1, players_stored[1].getId());
        EXPECT_STREQ("Enemy", players_stored[1].getName().c_str());
        EXPECT_EQ(100, players_stored[1].getHealth());
        EXPECT_EQ(100, players_stored[1].getMaxHealth());
        EXPECT_EQ(1, players_stored[1].getTeamId());
        EXPECT_EQ(100, players_stored[1].getScore());
    }
    else if (players_stored[1].getId() == 0)
    {
        EXPECT_STREQ("Myself", players_stored[1].getName().c_str());
        EXPECT_EQ(50, players_stored[1].getHealth());
        EXPECT_EQ(100, players_stored[1].getMaxHealth());
        EXPECT_EQ(0, players_stored[1].getTeamId());
        EXPECT_EQ(0, players_stored[1].getScore());

        EXPECT_EQ(1, players_stored[0].getId());
        EXPECT_STREQ("Enemy", players_stored[0].getName().c_str());
        EXPECT_EQ(100, players_stored[0].getHealth());
        EXPECT_EQ(100, players_stored[0].getMaxHealth());
        EXPECT_EQ(1, players_stored[0].getTeamId());
        EXPECT_EQ(100, players_stored[0].getScore());
    }
    else
    {
        //-- Break test
        ASSERT_TRUE(false);
    }

}

TEST_F( RdMentalMapTest, MyselfPointsToMe)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for two players:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Update players:
    std::vector<RdPlayer> players;
    players.push_back(*player1);
    players.push_back(*player2);

    ASSERT_TRUE(mentalMap->updatePlayers(players));

    //-- Check players stored:
    std::vector<RdPlayer> players_stored = mentalMap->getPlayers();
    ASSERT_EQ(2, (int) players_stored.size());

    //-- Check if myself is me:
    RdPlayer me = mentalMap->getMyself();
    EXPECT_EQ(0, me.getId());
    EXPECT_STREQ("Myself", me.getName().c_str());
    EXPECT_EQ(0, me.getTeamId());

}


TEST_F( RdMentalMapTest, RespawnWorks)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for one dead player:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Update players:
    player1->setHealth(0);
    std::vector<RdPlayer> players;
    players.push_back(*player1);

    ASSERT_TRUE(mentalMap->updatePlayers(players));

    ASSERT_TRUE(mentalMap->respawn());

    //-- Check players stored:
    std::vector<RdPlayer> players_stored = mentalMap->getPlayers();
    ASSERT_EQ(1, (int) players_stored.size());

    //-- Check if myself is me:
    RdPlayer me = mentalMap->getMyself();
    EXPECT_EQ(0, me.getId());
    ASSERT_EQ(me.getMaxHealth(), me.getHealth());
}

//---------------------------------------------------------------------------------------
//-- Enemy-related
//---------------------------------------------------------------------------------------

TEST_F( RdMentalMapTest, UpdateTargetsUpdateTargets)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for two players:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Update players:
    std::vector<RdPlayer> players;
    players.push_back(*player1);
    players.push_back(*player2);

    ASSERT_TRUE(mentalMap->updatePlayers(players));


    //-- Update targets:
    std::vector<RdTarget> targets;
    targets.push_back(*target1);

    ASSERT_TRUE(mentalMap->updateTargets(targets));


    //-- Check targets:
    std::vector<RdTarget> targets_stored = mentalMap->getTargets();
    ASSERT_EQ(1, targets_stored.size());
    EXPECT_EQ(1, targets_stored[0].getPlayerId());
    EXPECT_EQ(100, targets_stored[0].getPos().x);
    EXPECT_EQ(100, targets_stored[0].getPos().y);
    EXPECT_EQ(50, targets_stored[0].getDimensions().x);
    EXPECT_EQ(50, targets_stored[0].getDimensions().y);
    EXPECT_EQ(100, targets_stored[0].getBelief());

}

TEST_F( RdMentalMapTest, UpdateEnemiesEventuallyDeletesEnemies)
{
    ASSERT_NE((RdMentalMap *)NULL, mentalMap);

    //-- Configure a mental map for two players:
    const int id = 0;
    ASSERT_FALSE(!mentalMap->configure(id));

    //-- Update players:
    std::vector<RdPlayer> players;
    players.push_back(*player1);
    players.push_back(*player2);
    players.push_back(*player3);
    ASSERT_TRUE(mentalMap->updatePlayers(players));


    //-- Update enemies:
    std::vector<RdTarget> targets;
    targets.push_back(*target1);
    targets.push_back(*target2);

    ASSERT_TRUE(mentalMap->updateTargets(targets));


    //-- Update with a single target until belief for the other target reaches 0:
    //-- (Belief decrease is hardcoded, and decreases by 10 each update)
    std::vector<RdTarget> new_target_vector;
    new_target_vector.push_back(*target2);
    ASSERT_TRUE(mentalMap->updateTargets(new_target_vector));

    //-- Check targets:
    RdTarget target_not_updated = mentalMap->getTarget(1);
    EXPECT_EQ(1, target_not_updated.getPlayerId());
    EXPECT_EQ(100, target_not_updated.getPos().x);
    EXPECT_EQ(100, target_not_updated.getPos().y);
    EXPECT_EQ(50, target_not_updated.getDimensions().x);
    EXPECT_EQ(50, target_not_updated.getDimensions().y);
    EXPECT_EQ(90, target_not_updated.getBelief());


    for (int i = 0; i < 9; i++)
        ASSERT_TRUE(mentalMap->updateTargets(new_target_vector));


    EXPECT_EQ(1, mentalMap->getTargets().size());
    EXPECT_EQ(2, mentalMap->getTargets()[0].getPlayerId());
    EXPECT_EQ(100, mentalMap->getTargets()[0].getBelief());

}



//---------------------------------------------------------------------------------------
//-- Thread-safe functions
//---------------------------------------------------------------------------------------

//TEST_F( RdMentalMapTest, TargetsLockedWhenModified)
//{
//    ASSERT_TRUE(false);
//}

//TEST_F( RdMentalMapTest, PlayersLockedWhenModified)
//{
//    ASSERT_FALSE(true);
//}

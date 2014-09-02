#include "RdMentalMap.hpp"

rd::RdMentalMap::RdMentalMap()
{
}

bool rd::RdMentalMap::configure(const int &player_id)
{
    this->my_id = player_id;
    this->myself = NULL;
}

rd::RdMentalMap::RdMentalMap(const int &player_id)
{
    this->my_id = player_id;
    this->myself = NULL;
}

std::vector<rd::RdEnemy> rd::RdMentalMap::getEnemies()
{
    std::vector<RdEnemy> enemy_vector;

    for( std::map<int, RdEnemy>::const_iterator it = enemies.begin(); it != enemies.end(); ++it)
    {
        enemy_vector.push_back(it->second);
    }

    return enemy_vector;

}

std::vector<rd::RdPlayer> rd::RdMentalMap::getPlayers()
{
    std::vector<RdPlayer> player_vector;

    for( std::map<int, RdPlayer>::const_iterator it = players.begin(); it != players.end(); ++it)
    {
        player_vector.push_back(it->second);
    }

    return player_vector;
}

rd::RdEnemy rd::RdMentalMap::getEnemy(const int &id)
{
    if ( enemies.find(id) != enemies.end() )
    {
        return enemies[id];
    }
    else
    {
        RD_ERROR("Enemy with id %i not found!!\n", id);
        RD_ERROR("Returning standard enemy\n");
        return RdEnemy();
    }
}

rd::RdPlayer rd::RdMentalMap::getPlayer(const int &id)
{
    if ( players.find(id) != players.end() )
    {
        return players[id];
    }
    else
    {
        RD_ERROR("Player with id %i not found!!\n", id);
        RD_ERROR("Returning standard player\n");
        return RdPlayer();
    }
}

rd::RdPlayer rd::RdMentalMap::getMyself()
{
    if (!myself)
    {
        RD_ERROR("Myself not found in mental map\n");
        RD_ERROR("Returning standard player\n");
        return RdPlayer();
    }
    else
    {
        return *myself;
    }
}

bool rd::RdMentalMap::updatePlayers(std::vector<rd::RdPlayer> new_player_vector)
{
    //-- Right now, this just replaces the players inside the mental map

    if ( new_player_vector.size() > max_num_players )
    {
        RD_ERROR( "Players in the update (%i) exceed the maximum number allowed in mental map (%i)\n",
                  (int) new_player_vector.size(), max_num_players);
        return false;
    }

    //-- Clear the pointer to myself (player)
    myself = NULL;

    for (int i = 0; i < (int) new_player_vector.size(); i++)
    {
        int id = new_player_vector[i].getId();
        players[id] = new_player_vector[i];

        if ( id == my_id)
            myself = &players[id];
    }

    //-- Update enemies to point to this new players!
    /// \todo Update enemies to point to this new players!

    return true;
}

bool rd::RdMentalMap::updateEnemies(std::vector<rd::RdEnemy> new_enemy_detections)
{
    //-- Reduce the belief of all the enemies and deletes them when belief is 0
    for( std::map<int, RdEnemy>::iterator it = enemies.begin(); it != enemies.end(); ++it)
    {
        if ( !it->second.reduceBelief(10) )
            enemies.erase(it);
    }

    //-- Check dimensions:
    if ( new_enemy_detections.size() > max_num_players-1 )
    {
        RD_ERROR( "Enemies in the update (%i) exceed the maximum number allowed in mental map (%i)\n",
                  (int) new_enemy_detections.size(), max_num_players-1);
        return false;
    }

    //-- Add new enemies / update old ones (just replacing):
    for (int i = 0; i < (int) new_enemy_detections.size(); i++)
    {
        int id = new_enemy_detections[i].getPlayerId();
        enemies[id] = new_enemy_detections[i];
    }

    return true;
}

bool rd::RdMentalMap::destroy()
{
    return true;
}

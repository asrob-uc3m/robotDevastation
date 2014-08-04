#include "RdMentalMap.hpp"



rd::RdMentalMap::RdMentalMap(const int &player_id, const int &max_num_players)
{
    this->my_id = player_id;
    this->max_num_players = max_num_players;
}

std::vector<rd::RdEnemy> rd::RdMentalMap::getEnemies()
{
    return std::vector<RdEnemy>();
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

rd::RdPlayer rd::RdMentalMap::getPlayer(const int &id)
{
    if ( players.find(id) != players.end() )
    {
        return players[id];
    }
    else
    {
        RD_ERROR("Player with id %i not found!!\n", id);
        RD_ERROR("Returning standard player");
        return RdPlayer(-1, "Dummy", 0, 0, -1, -1);
    }
}

bool rd::RdMentalMap::updatePlayers(std::vector<rd::RdPlayer> new_player_vector)
{
    //-- Right now, this just replaces the players inside the mental map
    if ( new_player_vector.size() > max_num_players )
    {
        RD_ERROR( "Players in the update (%i) exceed the maximum number allowed in mental map (%i)",
                  (int) new_player_vector.size(), max_num_players);
        return false;
    }

    for (int i = 0; i < (int) new_player_vector.size(); i++)
    {
        players[new_player_vector[i].getId()] = new_player_vector[i];
    }

    return true;
}

bool rd::RdMentalMap::destroy()
{
    return true;
}

// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_HPP__
#define __RD_MENTAL_MAP_HPP__

#include <map>
#include <vector>
#include <iterator>

#include "RdPlayer.hpp"
#include "RdEnemy.hpp"
//#include "RdVector2dBase.hpp"
#include "RdUtils.hpp"

namespace rd{

class RdMentalMap
{
    public:
        RdMentalMap(const int& player_id, const int& max_num_players );

        std::vector<RdEnemy> getEnemies();
        std::vector<RdPlayer> getPlayers();
        RdEnemy getEnemy(const int& id = -1);
        RdPlayer getPlayer(const int& id = -1);
        RdPlayer getMyself();

        bool updatePlayers(std::vector<RdPlayer> new_player_vector);
        bool updateEnemies(std::vector<RdEnemy> new_enemy_detections);

        bool destroy();

    private:
        std::map<int, RdEnemy> enemies;
        std::map<int, RdPlayer> players;

        int my_id;
        RdPlayer* myself;

        int max_num_players;

};

}

#endif //__RD_MENTAL_MAP_HPP__

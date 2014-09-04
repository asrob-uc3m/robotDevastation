// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_HPP__
#define __RD_MENTAL_MAP_HPP__

#include <map>
#include <vector>
#include <iterator>

#include "RdPlayer.hpp"
#include "RdTarget.hpp"
#include "RdUtils.hpp"

namespace rd{

class RdMentalMap
{
    public:
        static RdMentalMap * getMentalMap();
        static bool destroyMentalMap();

        bool configure(const int& player_id );

        std::vector<RdTarget> getTargets();
        std::vector<RdPlayer> getPlayers();
        RdTarget getTarget(const int& id = -1);
        RdPlayer getPlayer(const int& id = -1);
        RdPlayer getMyself();


        bool updatePlayers(std::vector<RdPlayer> new_player_vector);
        bool updateTargets(std::vector<RdTarget> new_target_detections);

        bool destroy();

    private:
        static RdMentalMap * mentalMapInstance;

        RdMentalMap();
        RdMentalMap(const int& player_id);

        std::map<int, RdTarget> targets;
        std::map<int, RdPlayer> players;

        int my_id;
        RdPlayer* myself;

};

}

#endif //__RD_MENTAL_MAP_HPP__

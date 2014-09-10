// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_HPP__
#define __RD_MENTAL_MAP_HPP__

#include <map>
#include <vector>
#include <iterator>

#include "RdPlayer.hpp"
#include "RdTarget.hpp"
#include "RdWeapon.hpp"
#include "RdUtils.hpp"
#include "RdMentalMapEventListener.hpp"
#include "RdNetworkEventListener.hpp"
#include "RdNetworkManager.hpp"


namespace rd{

class RdMentalMap : public RdNetworkEventListener
{
    public:
        //-- Creation and configuration
        static RdMentalMap * getMentalMap();
        static bool destroyMentalMap();

        bool configure(const int& player_id );

        //-- Interface to get data
        std::vector<RdTarget> getTargets();
        std::vector<RdPlayer> getPlayers();
        RdTarget getTarget(const int& id = -1);
        RdPlayer getPlayer(const int& id = -1);
        RdPlayer getMyself();

        //-- Weapon interface
        bool addWeapon(RdWeapon weapon);
        bool shoot();
        bool reload();

        //-- Functions to update data
        bool updatePlayers(std::vector<RdPlayer> new_player_vector);
        bool updateTargets(std::vector<RdTarget> new_target_detections);

        //-- Listeners
        bool addMentalMapEventListener( RdMentalMapEventListener * listener);

    private:
        static RdMentalMap * mentalMapInstance;
        RdMentalMap();

        std::map<int, RdTarget> targets;
        std::map<int, RdPlayer> players;
        std::vector<RdWeapon> weapons;
        int current_weapon;

        int my_id;
        RdPlayer* myself;

        RdAudioManager * audioManager;

        std::vector<RdMentalMapEventListener *> listeners;

        //-- Implementation of RdNetworkEventListener functions
        bool onDataArrived(RdPlayer player);
        bool onDataArrived(std::vector<RdPlayer> players);
};

}

#endif //__RD_MENTAL_MAP_HPP__

// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_EVENT_LISTENER_HPP__
#define __RD_MENTAL_MAP_EVENT_LISTENER_HPP__

#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"

namespace rd{

/**
 * @ingroup RdMentalMapLib
 *
 * @brief Interface for objects that can be notified of events related to changes on the data stored
 *
 * RdMentalMapEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
 */
class MentalMapEventListener
{
    public:
        /**
         * @brief This function will be called whenever a target is hit
         * @param target Target that was hit
         * @param player Player corresponding to the target hit
         * @param weapon Weapon used to hit the target
         */
        virtual bool onTargetHit(Target target, Player player, Weapon weapon) = 0;

        /**
         * @brief This function will be called whenever the player is respawned
         * @param player Player to respawn
         */
        virtual bool onRespawn(Player player) = 0;
};

}

#endif //-- __RD_MENTAL_MAP_EVENT_LISTENER_HPP__


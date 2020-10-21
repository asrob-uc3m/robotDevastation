// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MENTAL_MAP_EVENT_LISTENER_HPP__
#define __RD_MENTAL_MAP_EVENT_LISTENER_HPP__

#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"

namespace rd{

/**
 * @ingroup MentalMapLib
 *
 * @brief Interface for objects that can be notified of events related to changes on the data stored
 *
 * MentalMapEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
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
        virtual bool onTargetHit(const Target & target, const Player & player, const Weapon & weapon) = 0;

        /**
         * @brief This function will be called whenever the player is respawned
         * @param player Player to respawn
         */
        virtual bool onRespawn(const Player & player) = 0;

        virtual ~MentalMapEventListener() {}
};

}

#endif //-- __RD_MENTAL_MAP_EVENT_LISTENER_HPP__

// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_MANAGER_HPP__
#define __RD_NETWORK_MANAGER_HPP__

#include "RdPlayer.hpp"
#include "RdNetworkEventListener.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdNetworkLib
 *
 * @brief The RdNetworkLib library contains everything related to the communications with the RobotDevastation network
 */

/**
 * @ingroup RdNetworkLib
 *
 * @brief Manages the communications between the user and the RobotDevastation network
 *
 * RdNetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdNetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the RdNetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived)
 *
 */
class RdNetworkManager
{
    public:
        //-- Creation and configuration
        //--------------------------------------------------------------------------------------------
        //! @todo Change this singleton to the new singleton implementation with Register()

        virtual ~RdNetworkManager();

        //-- Listeners
        //--------------------------------------------------------------------------------------------
        //! @brief Adds a RdInputEventListener to the list of observers to be notified of events
        bool addNetworkEventListener( RdNetworkEventListener * listener );

        //! @brief Unregisters all the RdInputEventListener stored
        bool removeNetworkEventListeners();


        //-- Network API
        //--------------------------------------------------------------------------------------------
        //! @brief Communicate to the network that a player has been hit by the user
        virtual bool sendPlayerHit(RdPlayer player, int damage) = 0;

        //! @brief Log the user into the network
        virtual bool login(RdPlayer player) = 0;

        //! @brief Log the user out of the network
        virtual bool logout(RdPlayer player) = 0;

    protected:
        //! @brief Stores the unique instance of the RdNetworkManager
        static RdNetworkManager * networkManagerInstance;

        //! @brief Observers registered to be notified of network events
        std::vector<RdNetworkEventListener *> listeners;


};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

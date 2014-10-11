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
 * @brief The RdNetworkLib library contains the \ref RdNetworkManager base class and derived classes.
 */

/**
 * @ingroup RdNetworkLib
 *
 * @brief A base class for Network Managers.
 *
 */
class RdNetworkManager
{
    public:
        bool addNetworkEventListener( RdNetworkEventListener * listener );
        bool removeNetworkEventListeners();

        virtual bool sendPlayerHit(RdPlayer player, int damage) = 0;
        virtual bool login(RdPlayer player) = 0;
        virtual bool logout(RdPlayer player) = 0;

    protected:
        std::vector<RdNetworkEventListener *> listeners;
        static RdNetworkManager * networkManagerInstance;
};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

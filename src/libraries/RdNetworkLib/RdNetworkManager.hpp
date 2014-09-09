// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_MANAGER_HPP__
#define __RD_NETWORK_MANAGER_HPP__

#include "RdMacros.hpp"
#include "RdPlayer.hpp"
#include "RdNetworkEventListener.hpp"

namespace rd{

class RdNetworkManager
{
    public:
        static RdNetworkManager * getNetworkManager();
        static bool destroyNetworkManager();

        ~RdNetworkManager();

        bool addNetworkEventListener( RdNetworkEventListener * listener );

    protected:
        virtual bool sendPlayer(RdPlayer player);
        std::vector<RdNetworkEventListener *> listeners;
        static RdNetworkManager * networkManagerInstance;

    private:
        RdNetworkManager();




};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

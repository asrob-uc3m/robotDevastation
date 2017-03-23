// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_EVENT_LISTENER_HPP__
#define __RD_NETWORK_EVENT_LISTENER_HPP__

#include <vector>
#include "Player.hpp"

namespace rd{

class NetworkEventListener
{
    public:
        virtual bool onDataArrived(std::vector<Player> players) = 0;
};

}

#endif //-- __RD_NETWORK_EVENT_LISTENER_HPP__

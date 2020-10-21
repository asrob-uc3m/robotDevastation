// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_NETWORK_EVENT_LISTENER_HPP__
#define __RD_NETWORK_EVENT_LISTENER_HPP__

#include <vector>
#include "Player.hpp"

namespace rd{

/**
 * @ingroup NetworkLib
 *
 * @brief Interface for objects that can be notified of input events by the NetworkManager
 */
class NetworkEventListener
{
    public:
        virtual bool onDataArrived(const std::vector<Player> & players) = 0;

        virtual ~NetworkEventListener() {}
};

}

#endif //-- __RD_NETWORK_EVENT_LISTENER_HPP__

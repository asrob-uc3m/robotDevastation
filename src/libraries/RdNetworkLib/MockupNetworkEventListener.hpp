#ifndef _MOCKUP_NETWORK_EVENT_LISTENER_HPP_
#define _MOCKUP_NETWORK_EVENT_LISTENER_HPP_

#include "RdNetworkEventListener.hpp"

namespace rd {
/**
 * @brief Dummy RdNetworkEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received data and has a counter of
 * the incoming messages received
 *
 */

class MockupNetworkEventListener : public RdNetworkEventListener
{
    public:
        MockupNetworkEventListener();

        virtual bool onDataArrived(std::vector<RdPlayer> players);

        int getDataArrived();
        void resetDataArrived();

        std::vector<RdPlayer> getStoredPlayers();


    private:
        int data_arrived;
        std::vector<RdPlayer> stored_players;
};



}

#endif // _MOCKUP_NETWORK_EVENT_LISTENER_HPP_

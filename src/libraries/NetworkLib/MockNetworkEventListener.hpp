#ifndef _MOCKUP_NETWORK_EVENT_LISTENER_HPP_
#define _MOCKUP_NETWORK_EVENT_LISTENER_HPP_

#include "NetworkEventListener.hpp"

namespace rd {
/**
 * @brief Dummy NetworkEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received data and has a counter of
 * the incoming messages received
 *
 */

class MockupNetworkEventListener : public NetworkEventListener
{
    public:
        MockupNetworkEventListener();

        virtual bool onDataArrived(std::vector<Player> players);

        int getDataArrived();
        void resetDataArrived();

        std::vector<Player> getStoredPlayers();


    private:
        int data_arrived;
        std::vector<Player> stored_players;
};



}

#endif // _MOCKUP_NETWORK_EVENT_LISTENER_HPP_

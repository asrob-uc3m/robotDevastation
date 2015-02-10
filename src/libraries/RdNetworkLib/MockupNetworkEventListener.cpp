#include "MockupNetworkEventListener.hpp"


rd::MockupNetworkEventListener::MockupNetworkEventListener()
{
    data_arrived = 0;
}

bool rd::MockupNetworkEventListener::onDataArrived(std::vector<rd::RdPlayer> players)
{
    stored_players = players;
    data_arrived++;
}

int rd::MockupNetworkEventListener::getDataArrived()
{
    return data_arrived;
}

void rd::MockupNetworkEventListener::resetDataArrived()
{
    data_arrived = 0;
}

std::vector<rd::RdPlayer> rd::MockupNetworkEventListener::getStoredPlayers()
{
    return stored_players;
}

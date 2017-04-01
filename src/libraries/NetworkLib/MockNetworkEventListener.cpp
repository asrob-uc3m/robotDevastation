#include "MockNetworkEventListener.hpp"


rd::MockNetworkEventListener::MockNetworkEventListener()
{
    data_arrived = 0;
}

bool rd::MockNetworkEventListener::onDataArrived(const std::vector<rd::Player> & players)
{
    stored_players = players;
    data_arrived++;
    return true;
}

int rd::MockNetworkEventListener::getDataArrived() const
{
    return data_arrived;
}

void rd::MockNetworkEventListener::resetDataArrived()
{
    data_arrived = 0;
}

std::vector<rd::Player> rd::MockNetworkEventListener::getStoredPlayers() const
{
    return stored_players;
}

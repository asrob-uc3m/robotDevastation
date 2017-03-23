#include "MockNetworkEventListener.hpp"


rd::MockNetworkEventListener::MockNetworkEventListener()
{
    data_arrived = 0;
}

bool rd::MockNetworkEventListener::onDataArrived(std::vector<rd::Player> players)
{
    stored_players = players;
    data_arrived++;
    return true;
}

int rd::MockNetworkEventListener::getDataArrived()
{
    return data_arrived;
}

void rd::MockNetworkEventListener::resetDataArrived()
{
    data_arrived = 0;
}

std::vector<rd::Player> rd::MockNetworkEventListener::getStoredPlayers()
{
    return stored_players;
}

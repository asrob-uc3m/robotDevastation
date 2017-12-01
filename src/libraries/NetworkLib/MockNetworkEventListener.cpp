// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockNetworkEventListener.hpp"

rd::MockNetworkEventListener::MockNetworkEventListener()
{
    data_arrived = 0;
}

bool rd::MockNetworkEventListener::onDataArrived(const std::vector<Player> & players)
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

const std::vector<rd::Player> & rd::MockNetworkEventListener::getStoredPlayers() const
{
    return stored_players;
}

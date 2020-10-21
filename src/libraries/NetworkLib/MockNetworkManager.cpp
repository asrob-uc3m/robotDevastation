// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockNetworkManager.hpp"

#include <ColorDebug.h>

//-- Initialize static members
rd::MockNetworkManager * rd::MockNetworkManager::uniqueInstance = NULL;
const std::string rd::MockNetworkManager::id = "MOCK";


bool rd::MockNetworkManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockNetworkManager();
    }

    return Register( uniqueInstance, id);
}

rd::MockNetworkManager::~MockNetworkManager()
{
    uniqueInstance = NULL;
}

bool rd::MockNetworkManager::start()
{
    if (player.getId() == -1)
        return false;

    stopped = false;
    return true;
}

bool rd::MockNetworkManager::stop()
{
    stopped = true;
    return true;
}

bool rd::MockNetworkManager::configure(const std::string & parameter, const Player & value)
{
    if (parameter.compare("player") == 0)
    {
        player = value;
        return true;
    }

    return NetworkManager::configure(parameter, value);
}

bool rd::MockNetworkManager::sendPlayerHit(const Player & player, int damage)
{
    int id = player.getId();

    if (!logged_in)
    {
        CD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    if ( players_dic.find(id) != players_dic.end() )  // if found, we can shoot
    {
        int new_health = players_dic[id].getHealth() - damage;

        if (new_health < 0)
            new_health = 0;

        players_dic[id].setHealth(new_health);

    }
    else
    {
        CD_ERROR("Target does not exist, id: %d.\n",id);
        return false;
    }

    sendPlayerData();

    return true;
}

bool rd::MockNetworkManager::login()
{

    int loginId = player.getId();

    if (logged_in)
    {
        CD_ERROR("Already logged, id: %d.\n",loginId);
        return false;
    }

    if ( players_dic.find(loginId) == players_dic.end() )  // if not found, we can create
    {
        players_dic[loginId] = player;
        logged_in = true;
    }
    else
    {
        CD_ERROR("Already logged, id: %d.\n",loginId);
        return false;
    }

    sendPlayerData();

    return true;
}

bool rd::MockNetworkManager::logout()
{
    int id = player.getId();

    if (!logged_in)
    {
        CD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    std::map<int, Player>::iterator it = players_dic.find(id);

    if ( it != players_dic.end() )  // if  found, we can erase it
    {
        players_dic.erase(it);
        logged_in = false;
    }
    else
    {
        CD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    sendPlayerData();

    return true;
}

bool rd::MockNetworkManager::keepAlive()
{
    return true; //-- Not really required in mock object
}

bool rd::MockNetworkManager::isLoggedIn() const
{
    return logged_in;
}

bool rd::MockNetworkManager::isStopped() const
{
    return stopped;
}

bool rd::MockNetworkManager::setPlayerData(const std::vector<Player> & players)
{
    players_dic.clear();

    for (int i = 0; i < (int) players.size(); i++)
    {
        int id = players[i].getId();
        players_dic[id] = players[i];
    }

    sendPlayerData();
    return true;
}

std::vector<rd::Player> rd::MockNetworkManager::getPlayerData() const
{
    std::vector<Player> player_vector;

    for( std::map<int, Player>::const_iterator it = players_dic.begin(); it != players_dic.end(); ++it)
    {
        player_vector.push_back(it->second);
    }

    return player_vector;
}

bool rd::MockNetworkManager::sendPlayerData()
{
    //-- Notify listeners
    for (std::vector<NetworkEventListener *>::iterator it = listeners.begin(); it != listeners.end(); ++it)
    {
        (*it)->onDataArrived(getPlayerData());
    }
    return true;
}

bool rd::MockNetworkManager::setLoggedIn(bool logged_in)
{
    if (this->logged_in != logged_in)
    {
        this->logged_in = logged_in;
        return true;
    }
    else
    {
        CD_WARNING("Already in that state: %d", logged_in);
        return false;
    }
}

rd::MockNetworkManager::MockNetworkManager()
{
    logged_in = false;
    stopped = true;
}

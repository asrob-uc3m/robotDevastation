#include "MockupNetworkManager.hpp"

//-- Initialize static members
rd::MockupNetworkManager * rd::MockupNetworkManager::uniqueInstance = NULL;
const std::string rd::MockupNetworkManager::id = "MOCKUP";


bool rd::MockupNetworkManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockupNetworkManager();
    }

    return Register( uniqueInstance, id);
}

rd::MockupNetworkManager::~MockupNetworkManager()
{
    uniqueInstance = NULL;
}

bool rd::MockupNetworkManager::start()
{
    stopped = false;
}

bool rd::MockupNetworkManager::stop()
{
    stopped = true;
}

bool rd::MockupNetworkManager::configure(std::string parameter, std::string value)
{
    return true;
}

bool rd::MockupNetworkManager::sendPlayerHit(rd::RdPlayer player, int damage)
{
    int id = player.getId();

    if (!logged_in)
    {
        RD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    if ( players_dic.find(id) != players_dic.end() )  // if found, we can shoot
    {
        int new_health = players_dic.operator[](id).getHealth() - damage;

        if (new_health < 0)
            new_health = 0;

        players_dic.operator[](id).setHealth(new_health);

    }
    else
    {
        RD_ERROR("Target does not exist, id: %d.\n",id);
        return false;
    }

    return true;
}

bool rd::MockupNetworkManager::login(rd::RdPlayer player)
{

    int loginId = player.getId();

    if (logged_in)
    {
        RD_ERROR("Already logged, id: %d.\n",loginId);
        return false;
    }

    if ( players_dic.find(loginId) == players_dic.end() )  // if not found, we can create
    {
        players_dic.operator[](loginId) = player;
        logged_in = true;
    }
    else
    {
        RD_ERROR("Already logged, id: %d.\n",loginId);
        return false;
    }

    return true;
}

bool rd::MockupNetworkManager::logout(rd::RdPlayer player)
{
    int id = player.getId();

    if (!logged_in)
    {
        RD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    std::map<int, RdPlayer>::iterator it = players_dic.find(id);

    if ( it != players_dic.end() )  // if  found, we can erase it
    {
        players_dic.erase(it);
        logged_in = false;
    }
    else
    {
        RD_ERROR("Not logged in, id: %d.\n",id);
        return false;
    }

    return true;
}

bool rd::MockupNetworkManager::isLoggedIn()
{
    return logged_in;
}

bool rd::MockupNetworkManager::isStopped()
{
    return stopped;
}

bool rd::MockupNetworkManager::setPlayerData(std::vector<rd::RdPlayer> players)
{
    players_dic.clear();

    for (int i = 0; i < (int) players.size(); i++)
    {
        int id = players[i].getId();
        this->players_dic[id] = players[i];
    }

    return true;
}

std::vector<rd::RdPlayer> rd::MockupNetworkManager::getPlayerData()
{
    std::vector<RdPlayer> player_vector;

    for( std::map<int, RdPlayer>::const_iterator it = players_dic.begin(); it != players_dic.end(); ++it)
    {
        player_vector.push_back(it->second);
    }

    return player_vector;
}

bool rd::MockupNetworkManager::sendPlayerData()
{
    //-- Notify listeners
    for(int i = 0; i < listeners.size(); i++)
        listeners[i]->onDataArrived(this->getPlayerData());
}

bool rd::MockupNetworkManager::setLoggedIn(bool logged_in)
{
    if (this->logged_in != logged_in)
    {
        this->logged_in = logged_in;
        return true;
    }
    else
    {
        RD_WARNING("Already in that state: %d", logged_in);
        return false;
    }
}

rd::MockupNetworkManager::MockupNetworkManager()
{
    logged_in = false;
    stopped = true;
}


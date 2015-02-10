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
    return false;
}

bool rd::MockupNetworkManager::login(rd::RdPlayer player)
{
    return false;
}

bool rd::MockupNetworkManager::logout(rd::RdPlayer player)
{
    return false;
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
    this->players = players;
}

std::vector<rd::RdPlayer> rd::MockupNetworkManager::getPlayerData()
{
    return players;
}

bool rd::MockupNetworkManager::sendPlayerData()
{
    //-- Notify listeners
    for(int i = 0; i < listeners.size(); i++)
        listeners[i]->onDataArrived(players);
}

rd::MockupNetworkManager::MockupNetworkManager()
{

}


#include "NetworkManager.hpp"

//-- Static members initialization:
rd::NetworkManager * rd::NetworkManager::networkManagerInstance = NULL;
std::string rd::NetworkManager::currentId = "";
std::map<std::string, rd::NetworkManager *> rd::NetworkManager::networkManagerRegistry = std::map<std::string, rd::NetworkManager *>();

rd::NetworkManager *rd::NetworkManager::getNetworkManager()
{
    if (networkManagerInstance == NULL )
    {
        if(!networkManagerRegistry.empty() )
        {
            //-- Install the first RdNetworkManager found
            currentId = networkManagerRegistry.begin()->first;
            networkManagerInstance = networkManagerRegistry.begin()->second;
        }
        else
        {
            RD_ERROR("No RdNetworkManager registered!! Returning null\n");
        }
    }

    return networkManagerInstance;
}

rd::NetworkManager *rd::NetworkManager::getNetworkManager(std::string id)
{
    if (networkManagerInstance == NULL )
    {
        if( networkManagerRegistry.find(id) != networkManagerRegistry.end() )
        {
            currentId = id;
            networkManagerInstance = networkManagerRegistry[id];
        }
        else
        {
            RD_ERROR("RdImageManager %s not found! (Maybe it was not registered?)\n", id.c_str());
        }
    }

    return networkManagerInstance;
}

bool rd::NetworkManager::destroyNetworkManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (networkManagerInstance != NULL)
    {
        networkManagerInstance->stop();

        delete networkManagerInstance;
        networkManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, NetworkManager *>::iterator it = networkManagerRegistry.find(currentId);
        if (it != networkManagerRegistry.end())
            networkManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered ImageManagers
    for ( std::map<std::string, NetworkManager *>::iterator it = networkManagerRegistry.begin();
          it != networkManagerRegistry.end(); ++it)
    {
        NetworkManager * currentManager = it->second;

        if (currentManager != NULL)
        {
            currentManager->stop();

            delete currentManager;
            currentManager = NULL;
         }

         //-- Remove it also from the registry
         networkManagerRegistry.erase(it);
    }


    return true;
}

rd::NetworkManager::~NetworkManager() { }

bool rd::NetworkManager::addNetworkEventListener(rd::NetworkEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::NetworkManager::removeNetworkEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::NetworkManager::configure(std::string parameter, Player value)
{
    return true;
}

bool rd::NetworkManager::onTargetHit(rd::Target target, rd::Player player, rd::Weapon weapon)
{
    return sendPlayerHit(player, weapon.getDamage());
}

bool rd::NetworkManager::onRespawn(rd::Player player)
{
    return sendPlayerHit(player, -1*player.getMaxHealth()); //-- This is a quick and dirty hack
}

bool rd::NetworkManager::Register(rd::NetworkManager *manager, std::string id)
{
    if ( networkManagerRegistry.find(id) == networkManagerRegistry.end())
    {
        if (manager != NULL)
        {
            networkManagerRegistry[id] = manager;
            return true;
        }
        else
        {
            RD_ERROR("Trying to register a NULL manager with id \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        RD_ERROR( "RdImageManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

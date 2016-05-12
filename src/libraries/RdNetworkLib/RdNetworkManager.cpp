#include "RdNetworkManager.hpp"

//-- Static members initialization:
rd::RdNetworkManager * rd::RdNetworkManager::networkManagerInstance = NULL;
std::string rd::RdNetworkManager::currentId = "";
std::map<std::string, rd::RdNetworkManager *> rd::RdNetworkManager::networkManagerRegistry = std::map<std::string, rd::RdNetworkManager *>();


rd::RdNetworkManager *rd::RdNetworkManager::getNetworkManager()
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

rd::RdNetworkManager *rd::RdNetworkManager::getNetworkManager(std::string id)
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

bool rd::RdNetworkManager::destroyNetworkManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (networkManagerInstance != NULL)
    {
        networkManagerInstance->stop();

        delete networkManagerInstance;
        networkManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, RdNetworkManager *>::iterator it = networkManagerRegistry.find(currentId);
        if (it != networkManagerRegistry.end())
            networkManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered ImageManagers
    for ( std::map<std::string, RdNetworkManager *>::iterator it = networkManagerRegistry.begin();
          it != networkManagerRegistry.end(); ++it)
    {
        RdNetworkManager * currentManager = it->second;

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

rd::RdNetworkManager::~RdNetworkManager() { }

bool rd::RdNetworkManager::addNetworkEventListener(rd::RdNetworkEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::RdNetworkManager::removeNetworkEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::RdNetworkManager::configure(std::string parameter, std::string value)
{
    return true;
}

bool rd::RdNetworkManager::onTargetHit(rd::RdTarget target, rd::RdPlayer player, rd::RdWeapon weapon)
{
    return sendPlayerHit(player, weapon.getDamage());
}

bool rd::RdNetworkManager::Register(rd::RdNetworkManager *manager, std::string id)
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

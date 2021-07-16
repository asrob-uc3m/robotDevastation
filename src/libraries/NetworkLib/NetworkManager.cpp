// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "NetworkManager.hpp"

#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"

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
            //-- Install the first NetworkManager found
            currentId = networkManagerRegistry.begin()->first;
            networkManagerInstance = networkManagerRegistry.begin()->second;
        }
        else
        {
            yCError(RD_NET) << "No NetworkManager registered, returning null";
        }
    }

    return networkManagerInstance;
}

rd::NetworkManager *rd::NetworkManager::getNetworkManager(const std::string & id)
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
            yCError(RD_NET) << "ImageManager" << id << "not found (maybe it was not registered?)";
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

bool rd::NetworkManager::addNetworkEventListener(NetworkEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::NetworkManager::removeNetworkEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::NetworkManager::configure(const std::string & parameter, const Player & value)
{
    return true;
}

bool rd::NetworkManager::onTargetHit(const Target & target, const Player & player, const Weapon & weapon)
{
    return sendPlayerHit(player, weapon.getDamage());
}

bool rd::NetworkManager::onRespawn(const Player & player)
{
    return sendPlayerHit(player, -1*player.getMaxHealth()); //-- This is a quick and dirty hack
}

bool rd::NetworkManager::Register(NetworkManager *manager, const std::string & id)
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
            yCError(RD_NET) << "Trying to register a NULL manager with id" << id;
            return false;
        }
    }
    else
    {
        yCError(RD_NET) << "ImageManager with id" << id << "was already registered";
        return false;
    }

}

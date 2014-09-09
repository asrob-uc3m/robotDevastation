#include "RdNetworkManager.hpp"

//-- This is very important:
rd::RdNetworkManager * rd::RdNetworkManager::networkManagerInstance = NULL;

rd::RdNetworkManager *rd::RdNetworkManager::getNetworkManager()
{
    if( networkManagerInstance == NULL )
        networkManagerInstance = new RdNetworkManager();

    return networkManagerInstance;
}

bool rd::RdNetworkManager::destroyNetworkManager()
{
    if (networkManagerInstance == NULL)
        return false;

    delete networkManagerInstance;
    networkManagerInstance = NULL;

    return true;
}

rd::RdNetworkManager::~RdNetworkManager()
{

}

bool rd::RdNetworkManager::addNetworkEventListener(rd::RdNetworkEventListener *listener)
{
    listeners.push_back(listener);
}

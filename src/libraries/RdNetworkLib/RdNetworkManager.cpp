#include "RdNetworkManager.hpp"

//-- This is very important (for the singleton to work correctly)
rd::RdNetworkManager * rd::RdNetworkManager::networkManagerInstance = NULL;

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

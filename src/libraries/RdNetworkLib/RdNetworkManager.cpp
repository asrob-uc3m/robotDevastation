#include "RdNetworkManager.hpp"

//-- This is very important:
rd::RdNetworkManager * rd::RdNetworkManager::networkManagerInstance = NULL;

bool rd::RdNetworkManager::addNetworkEventListener(rd::RdNetworkEventListener *listener)
{
    listeners.push_back(listener);
}

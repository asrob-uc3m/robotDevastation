#include "RdMouseManager.hpp"


rd::RdMouseManager::RdMouseManager()
{
    //-- Init SDL
}

rd::RdMouseManager::~RdMouseManager()
{
    //-- Close SDL
}

bool rd::RdMouseManager::start()
{
    //-- Start input thread
}

bool rd::RdMouseManager::setInputEventListeners(std::vector<rd::RdInputEventListener *> *listeners)
{
    this->listeners = listeners;
}

bool rd::RdMouseManager::update()
{
    //-- Call listeners when event occured
}

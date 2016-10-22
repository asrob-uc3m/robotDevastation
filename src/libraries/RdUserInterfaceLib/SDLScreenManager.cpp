#include "SDLScreenManager.hpp"

//-- This is very important:
rd::SDLScreenManager * rd::SDLScreenManager::uniqueInstance = NULL;
const std::string rd::SDLScreenManager::id = "SDL";

rd::SDLScreenManager::SDLScreenManager()
{
    stopped = true;
}

int rd::SDLScreenManager::initSDL()
{
    return 1;
}

int rd::SDLScreenManager::cleanSDL()
{
    return 1;
}

bool rd::SDLScreenManager::start()
{
    stopped = false;
    return true;
}

bool rd::SDLScreenManager::stop()
{
    stopped = true;
    return true;
}

bool rd::SDLScreenManager::isStopped()
{
    return stopped;
}

bool rd::SDLScreenManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new SDLScreenManager();
    }

    return Register( uniqueInstance, id);
}

rd::SDLScreenManager::~SDLScreenManager()
{
    //-- Stop this manager
    this->stop();
    uniqueInstance = NULL;
}

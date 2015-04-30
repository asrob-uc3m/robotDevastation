#include "MockupInputManager.hpp"

//-- Initialize static members
rd::MockupInputManager * rd::MockupInputManager::uniqueInstance = NULL;
const std::string rd::MockupInputManager::id = "MOCKUP";

bool rd::MockupInputManager::sendKeyPress(RdKey key)
{
    if (stopped)
        return false;

    for ( int i = 0; i < (int)listeners.size(); i++)
    {
        listeners.at(i)->onKeyUp(key);
        listeners.at(i)->onKeyDown(key);
    }

    return true;
}

rd::MockupInputManager::MockupInputManager()
{
    stopped = true;
}

rd::MockupInputManager::~MockupInputManager()
{
    uniqueInstance = NULL;
}

bool rd::MockupInputManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockupInputManager();
    }

    return Register(uniqueInstance, id);
}

bool rd::MockupInputManager::stop()
{
    stopped = true;
    return true;
}

bool rd::MockupInputManager::isStopped()
{
    return stopped;
}


bool rd::MockupInputManager::start()
{
    stopped = false;
    return true;
}

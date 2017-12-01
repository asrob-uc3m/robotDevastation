// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockInputManager.hpp"

//-- Initialize static members
rd::MockInputManager * rd::MockInputManager::uniqueInstance = NULL;
const std::string rd::MockInputManager::id = "MOCK";

bool rd::MockInputManager::sendKeyPress(const Key & key)
{
    if (stopped)
        return false;

    for (int i = 0; i < (int)listeners.size(); i++)
    {
        listeners.at(i)->onKeyUp(key);
        listeners.at(i)->onKeyDown(key);
    }

    return true;
}

bool rd::MockInputManager::sendKeyUp(const Key & key)
{
    if (stopped)
        return false;

    for (int i = 0; i < (int)listeners.size(); i++)
        listeners.at(i)->onKeyUp(key);

    return true;
}

bool rd::MockInputManager::sendKeyDown(const Key & key)
{
    if (stopped)
        return false;

    for (int i = 0; i < (int)listeners.size(); i++)
        listeners.at(i)->onKeyDown(key);

    return true;
}

bool rd::MockInputManager::sendWindowEvent(const WindowEvent & event)
{
    if (stopped)
        return false;

    for (int i = 0; i < (int)listeners.size(); i++)
        listeners.at(i)->onWindowEvent(event);

    return true;
}

rd::MockInputManager::MockInputManager()
{
    stopped = true;
}

rd::MockInputManager::~MockInputManager()
{
    uniqueInstance = NULL;
}

bool rd::MockInputManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockInputManager();
    }

    return Register(uniqueInstance, id);
}

bool rd::MockInputManager::stop()
{
    stopped = true;
    return true;
}

bool rd::MockInputManager::isStopped() const
{
    return stopped;
}

int rd::MockInputManager::getNumListeners()
{
    return listeners.size();
}


bool rd::MockInputManager::start()
{
    stopped = false;
    return true;
}

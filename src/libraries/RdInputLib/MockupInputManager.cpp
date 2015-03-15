#include "MockupInputManager.hpp"

//-- Initialize static members
rd::MockupInputManager * rd::MockupInputManager::uniqueInstance = NULL;
const std::string rd::MockupInputManager::id = "MOCKUP";

bool rd::MockupInputManager::sendKeyPress(rd::RdKey &key)
{
    return false;
}

rd::MockupInputManager::MockupInputManager()
{
}

bool rd::MockupInputManager::RegisterManager()
{
    return false;
}

bool rd::MockupInputManager::stop()
{
    return false;
}


bool rd::MockupInputManager::start()
{
    return false;
}

#include "RdInputManager.hpp"


//-- This is very important:
rd::RdInputManager * rd::RdInputManager::inputManagerInstance = NULL;

rd::RdInputManager *rd::RdInputManager::getInputManager()
{
    if( inputManagerInstance == NULL )
        inputManagerInstance = new RdInputManager();

    return inputManagerInstance;
}

bool rd::RdInputManager::destroyInputManager()
{
    if (inputManagerInstance == NULL)
        return false;

    delete inputManagerInstance;
    inputManagerInstance = NULL;

    return true;
}

rd::RdInputManager::~RdInputManager()
{
    //-- Destroy submodules:
    delete mouseManager;
    mouseManager = NULL;
}

bool rd::RdInputManager::start()
{
    //-- Set listeners to submodules:
    mouseManager->setInputEventListeners(&listeners);

    //-- Start submodules:
    mouseManager->start();
}

bool rd::RdInputManager::addInputEventListener(rd::RdInputEventListener *listener)
{
    listeners.push_back(listener);
}

rd::RdInputManager::RdInputManager()
{
    //-- Create submodules:
    mouseManager = new RdMouseManager();
}

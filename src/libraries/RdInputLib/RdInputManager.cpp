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
    delete keyboardManager;
    keyboardManager = NULL;
}

bool rd::RdInputManager::start()
{
    //-- Set listeners to submodules:
    keyboardManager->setInputEventListeners(&listeners);

    //-- Start submodules:
    keyboardManager->start();
}

bool rd::RdInputManager::addInputEventListener(rd::RdInputEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::RdInputManager::removeInputEventListeners()
{
    listeners.clear();
    return true;
}

rd::RdInputManager::RdInputManager()
{
    //-- Create submodules:
    keyboardManager = new RdKeyboardManager();
}

// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "InputManager.hpp"

#include <yarp/os/LogStream.h>

//-- This is very important:
rd::InputManager * rd::InputManager::inputManagerInstance = NULL;
std::string rd::InputManager::currentId = "";
std::map<std::string, rd::InputManager *> rd::InputManager::inputManagerRegistry = std::map<std::string, rd::InputManager *>();


rd::InputManager *rd::InputManager::getInputManager()
{
    if (inputManagerInstance == NULL )
    {
        if(!inputManagerRegistry.empty() )
        {
            //-- Install the first InputManager found
            currentId = inputManagerRegistry.begin()->first;
            inputManagerInstance = inputManagerRegistry.begin()->second;
        }
        else
        {
            yError() << "No InputManager registered, returning null";
        }
    }

    return inputManagerInstance;
}

rd::InputManager *rd::InputManager::getInputManager(const std::string & id)
{
    if (inputManagerInstance == NULL )
    {
        if( inputManagerRegistry.find(id) != inputManagerRegistry.end() )
        {
            currentId = id;
            inputManagerInstance = inputManagerRegistry[id];
        }
        else
        {
            yError() << "InputManager" << id << "not found (maybe it was not registered?)";
        }
    }

    return inputManagerInstance;
}

bool rd::InputManager::destroyInputManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (inputManagerInstance != NULL)
    {
        inputManagerInstance->stop();

        delete inputManagerInstance;
        inputManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, InputManager *>::iterator it = inputManagerRegistry.find(currentId);
        if (it != inputManagerRegistry.end())
            inputManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered InputManagers
    for ( std::map<std::string, InputManager *>::iterator it = inputManagerRegistry.begin();
          it != inputManagerRegistry.end(); ++it)
    {
        InputManager * currentManager = it->second;

        if (currentManager != NULL)
        {
            currentManager->stop();

            delete currentManager;
            currentManager = NULL;
         }

         //-- Remove it also from the registry
         inputManagerRegistry.erase(it);
    }

    return true;
}

rd::InputManager::~InputManager()
{
}

bool rd::InputManager::addInputEventListener(InputEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::InputManager::removeInputEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::InputManager::configure(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::InputManager::refreshEvents()
{
    return true;
}

bool rd::InputManager::Register(InputManager *manager, const std::string & id)
{
    if ( inputManagerRegistry.find(id) == inputManagerRegistry.end())
    {
        if (manager != NULL)
        {
            inputManagerRegistry[id] = manager;
            return true;
        }
        else
        {
            yError() << "Trying to register a NULL manager with id" << id;
            return false;
        }
    }
    else
    {
        yError() << "InputManager with id" << id << "was already registered";
        return false;
    }
}

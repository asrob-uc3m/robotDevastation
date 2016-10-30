#include "RdInputManager.hpp"


//-- This is very important:
rd::RdInputManager * rd::RdInputManager::inputManagerInstance = NULL;
std::string rd::RdInputManager::currentId = "";
std::map<std::string, rd::RdInputManager *> rd::RdInputManager::inputManagerRegistry = std::map<std::string, rd::RdInputManager *>();


rd::RdInputManager *rd::RdInputManager::getInputManager()
{
    if (inputManagerInstance == NULL )
    {
        if(!inputManagerRegistry.empty() )
        {
            //-- Install the first RdInputManager found
            currentId = inputManagerRegistry.begin()->first;
            inputManagerInstance = inputManagerRegistry.begin()->second;
        }
        else
        {
            RD_ERROR("No RdInputManager registered!! Returning null\n");
        }
    }

    return inputManagerInstance;
}

rd::RdInputManager *rd::RdInputManager::getInputManager(std::string id)
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
            RD_ERROR("RdInputManager %s not found! (Maybe it was not registered?)\n", id.c_str());
        }
    }

    return inputManagerInstance;
}

bool rd::RdInputManager::destroyInputManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (inputManagerInstance != NULL)
    {
        inputManagerInstance->stop();

        delete inputManagerInstance;
        inputManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, RdInputManager *>::iterator it = inputManagerRegistry.find(currentId);
        if (it != inputManagerRegistry.end())
            inputManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered InputManagers
    for ( std::map<std::string, RdInputManager *>::iterator it = inputManagerRegistry.begin();
          it != inputManagerRegistry.end(); ++it)
    {
        RdInputManager * currentManager = it->second;

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

rd::RdInputManager::~RdInputManager()
{
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

bool rd::RdInputManager::configure(std::string parameter, std::string value)
{
    return true;
}

bool rd::RdInputManager::refreshEvents()
{
    RD_INFO("Base implementation, does nothing!\n");
    return true;
}

bool rd::RdInputManager::Register(rd::RdInputManager *manager, std::string id)
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
            RD_ERROR("Trying to register a NULL manager with id \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        RD_ERROR( "RdInputManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

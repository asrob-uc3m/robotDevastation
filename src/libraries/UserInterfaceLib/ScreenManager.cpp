// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "ScreenManager.hpp"

#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"

//-- This is very important:
rd::ScreenManager * rd::ScreenManager::screenManagerInstance = NULL;
std::string rd::ScreenManager::currentId = "";
std::map<std::string, rd::ScreenManager *> rd::ScreenManager::screenManagerRegistry = std::map<std::string, rd::ScreenManager *>();


rd::ScreenManager *rd::ScreenManager::getScreenManager()
{
    if (screenManagerInstance == NULL )
    {
        if(!screenManagerRegistry.empty() )
        {
            //-- Install the first ScreenManager found
            currentId = screenManagerRegistry.begin()->first;
            screenManagerInstance = screenManagerRegistry.begin()->second;
        }
        else
        {
            yCError(RD_UI) << "No ScreenManager registered, returning null";
        }
    }

    return screenManagerInstance;
}

rd::ScreenManager *rd::ScreenManager::getScreenManager(const std::string & id)
{
    if (screenManagerInstance == NULL )
    {
        if( screenManagerRegistry.find(id) != screenManagerRegistry.end() )
        {
            currentId = id;
            screenManagerInstance = screenManagerRegistry[id];
        }
        else
        {
            yCError(RD_UI) << "ScreenManager" << id << "not found (mMaybe it was not registered?)";
        }
    }

    return screenManagerInstance;
}

bool rd::ScreenManager::destroyScreenManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (screenManagerInstance != NULL)
    {
        screenManagerInstance->stop();

        delete screenManagerInstance;
        screenManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, ScreenManager *>::iterator it = screenManagerRegistry.find(currentId);
        if (it != screenManagerRegistry.end())
            screenManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered ScreenManagers
    for ( std::map<std::string, ScreenManager *>::iterator it = screenManagerRegistry.begin();
          it != screenManagerRegistry.end(); ++it)
    {
        ScreenManager * currentManager = it->second;

        if (currentManager != NULL)
        {
            currentManager->stop();

            delete currentManager;
            currentManager = NULL;
         }

         //-- Remove it also from the registry
         screenManagerRegistry.erase(it);
    }

    return true;
}

rd::ScreenManager::~ScreenManager()
{
}

bool rd::ScreenManager::configure(const std::string & parameter, const std::string & value)
{
    return true;
}

void rd::ScreenManager::setCurrentScreen(Screen *screen)
{
    this->screen = screen;
}

bool rd::ScreenManager::update(const std::string & parameter, const std::string & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, const Image & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, const Player & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, const std::vector<Player> & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, const std::vector<Target> & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, const Weapon & value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::Register(ScreenManager *manager, const std::string & id)
{
    if ( screenManagerRegistry.find(id) == screenManagerRegistry.end())
    {
        if (manager != NULL)
        {
            screenManagerRegistry[id] = manager;
            return true;
        }
        else
        {
            yCError(RD_UI) << "Trying to register a NULL ScreenManager with id" << id;
            return false;
        }
    }
    else
    {
        yCError(RD_UI) << "ScreenManager with id" << id << "was already registered";
        return false;
    }

}

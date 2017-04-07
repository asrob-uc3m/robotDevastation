#include "ScreenManager.hpp"
#include "Macros.hpp"

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
            RD_ERROR("No ScreenManager registered!! Returning null\n");
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
            RD_ERROR("ScreenManager %s not found! (Maybe it was not registered?)\n", id.c_str());
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

bool rd::ScreenManager::update(const std::string & parameter, Image value)
{
    return screen->update(parameter, value);
}

bool rd::ScreenManager::update(const std::string & parameter, Player value)
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

bool rd::ScreenManager::update(const std::string & parameter, Weapon value)
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
            RD_ERROR("Trying to register a NULL ScreenManager with id \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        RD_ERROR( "ScreenManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

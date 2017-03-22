#include "ImageManager.hpp"


//-- Static members initialization:
rd::RdImageManager * rd::RdImageManager::imageManagerInstance = NULL;
std::string rd::RdImageManager::currentId = "";
std::map<std::string, rd::RdImageManager *> rd::RdImageManager::imageManagerRegistry = std::map<std::string, rd::RdImageManager *>();


rd::RdImageManager *rd::RdImageManager::getImageManager()
{
    if (imageManagerInstance == NULL )
    {
        if(!imageManagerRegistry.empty() )
        {
            //-- Install the first RdImageManger found
            currentId = imageManagerRegistry.begin()->first;
            imageManagerInstance = imageManagerRegistry.begin()->second;
        }
        else
        {
            RD_ERROR("No RdImageManager registered!! Returning null\n");
        }
    }

    return imageManagerInstance;
}

rd::RdImageManager *rd::RdImageManager::getImageManager(std::string id)
{
    if (imageManagerInstance == NULL )
    {
        if( imageManagerRegistry.find(id) != imageManagerRegistry.end() )
        {
            currentId = id;
            imageManagerInstance = imageManagerRegistry[id];
        }
        else
        {
            RD_ERROR("RdImageManager %s not found! (Maybe it was not registered?)\n", id.c_str());
        }
    }

    return imageManagerInstance;
}

bool rd::RdImageManager::destroyImageManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (imageManagerInstance != NULL)
    {
        imageManagerInstance->stop();

        delete imageManagerInstance;
        imageManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, RdImageManager *>::iterator it = imageManagerRegistry.find(currentId);
        if (it != imageManagerRegistry.end())
            imageManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered ImageManagers
    for ( std::map<std::string, RdImageManager *>::iterator it = imageManagerRegistry.begin();
          it != imageManagerRegistry.end(); ++it)
    {
        RdImageManager * currentManager = it->second;

        if (currentManager != NULL)
        {
            currentManager->stop();

            delete currentManager;
            currentManager = NULL;
         }

         //-- Remove it also from the registry
         imageManagerRegistry.erase(it);
    }


    return true;
}

rd::RdImageManager::~RdImageManager()
{

}

bool rd::RdImageManager::addImageEventListener(rd::RdImageEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::RdImageManager::removeImageEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::RdImageManager::configure(std::string parameter, std::string value)
{
    return true;
}

bool rd::RdImageManager::Register(rd::RdImageManager *manager, std::string id)
{
    if ( imageManagerRegistry.find(id) == imageManagerRegistry.end())
    {
        if (manager != NULL)
        {
            imageManagerRegistry[id] = manager;
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
        RD_ERROR( "RdImageManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

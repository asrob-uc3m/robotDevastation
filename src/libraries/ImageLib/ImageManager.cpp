// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "ImageManager.hpp"

#include <ColorDebug.hpp>

//-- Static members initialization:
rd::ImageManager * rd::ImageManager::imageManagerInstance = NULL;
std::string rd::ImageManager::currentId = "";
std::map<std::string, rd::ImageManager *> rd::ImageManager::imageManagerRegistry = std::map<std::string, rd::ImageManager *>();

rd::ImageManager *rd::ImageManager::getImageManager()
{
    if (imageManagerInstance == NULL )
    {
        if(!imageManagerRegistry.empty() )
        {
            //-- Install the first ImageManager found
            currentId = imageManagerRegistry.begin()->first;
            imageManagerInstance = imageManagerRegistry.begin()->second;
        }
        else
        {
            CD_ERROR("No ImageManager registered!! Returning null\n");
        }
    }

    return imageManagerInstance;
}

rd::ImageManager *rd::ImageManager::getImageManager(const std::string & id)
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
            CD_ERROR("ImageManager %s not found! (Maybe it was not registered?)\n", id.c_str());
        }
    }

    return imageManagerInstance;
}

bool rd::ImageManager::destroyImageManager()
{
    //-- First, stop & delete the manager currently in use (if any)
    if (imageManagerInstance != NULL)
    {
        imageManagerInstance->stop();

        delete imageManagerInstance;
        imageManagerInstance = NULL;

        //-- Remove it also from the registry
        std::map<std::string, ImageManager *>::iterator it = imageManagerRegistry.find(currentId);
        if (it != imageManagerRegistry.end())
            imageManagerRegistry.erase(it);
    }

    //-- Destroy all the remaining registered ImageManagers
    for ( std::map<std::string, ImageManager *>::iterator it = imageManagerRegistry.begin();
          it != imageManagerRegistry.end(); ++it)
    {
        ImageManager * currentManager = it->second;

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

rd::ImageManager::~ImageManager()
{

}

bool rd::ImageManager::addImageEventListener(ImageEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::ImageManager::removeImageEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::ImageManager::configure(const std::string & parameter, const std::string & value)
{
    return true;
}

bool rd::ImageManager::Register(ImageManager *manager, const std::string & id)
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
            CD_ERROR("Trying to register a NULL manager with id \"%s\"\n", id.c_str());
            return false;
        }
    }
    else
    {
        CD_ERROR( "ImageManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

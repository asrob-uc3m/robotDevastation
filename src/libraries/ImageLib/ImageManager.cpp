#include "ImageManager.hpp"
#include "Macros.hpp"

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
            RD_ERROR("No ImageManager registered!! Returning null\n");
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
            RD_ERROR("ImageManager %s not found! (Maybe it was not registered?)\n", id.c_str());
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

bool rd::ImageManager::addImageEventListener(rd::ImageEventListener *listener)
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

bool rd::ImageManager::Register(rd::ImageManager *manager, const std::string & id)
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
        RD_ERROR( "ImageManager with id \"%s\" was already registered\n", id.c_str());
        return false;
    }

}

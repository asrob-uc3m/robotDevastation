#include "RdImageManager.hpp"


//-- This is very important:
rd::RdImageManager * rd::RdImageManager::imageManagerInstance = NULL;
std::map<std::string, rd::RdImageManager *> rd::RdImageManager::imageManagerRegistry = std::map<std::string, rd::RdImageManager *>();

rd::RdImageManager *rd::RdImageManager::getImageManager()
{
    if (imageManagerInstance == NULL )
    {
        if(!imageManagerRegistry.empty() )
        {
            //-- Install the first RdImageManger found
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

    if (imageManagerInstance == NULL)
        return false;

    imageManagerInstance->stop();

    delete imageManagerInstance;
    imageManagerInstance = NULL;

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

rd::RdImageManager::RdImageManager()
{

}

bool rd::RdImageManager::Register(rd::RdImageManager *manager, std::string id)
{
    if ( imageManagerRegistry.find(id) == imageManagerRegistry.end())
    {
        if (manager != NULL)
        {
            imageManagerRegistry[id] = manager;
        }
        else
        {
            RD_ERROR("Trying to register a NULL manager with id \"%s\"\n", id.c_str());
        }
    }
    else
    {
        RD_ERROR( "RdImageManager with id \"%s\" was already registered\n", id.c_str());
    }
}

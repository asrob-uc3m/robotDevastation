#include "RdYarpImageManager.hpp"


//-- Initialize static members
rd::RdYarpImageManager * rd::RdYarpImageManager::uniqueInstance = NULL;
const std::string rd::RdYarpImageManager::id = "YARP";


bool rd::RdYarpImageManager::start()
{
    return false;
}

bool rd::RdYarpImageManager::stop()
{
    return false;
}

bool rd::RdYarpImageManager::configure(std::string parameter, std::string value)
{
    return false;
}

rd::RdImage rd::RdYarpImageManager::getImage()
{
    return RdImage();
}

bool rd::RdYarpImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdYarpImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdYarpImageManager::~RdYarpImageManager()
{
    uniqueInstance = NULL;
}

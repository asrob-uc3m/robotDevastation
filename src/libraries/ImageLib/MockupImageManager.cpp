#include "MockupImageManager.hpp"


//-- Initialize static members
rd::RdMockupImageManager * rd::RdMockupImageManager::uniqueInstance = NULL;
const std::string rd::RdMockupImageManager::id = "MOCKUP";


bool rd::RdMockupImageManager::start()
{
    RD_DEBUG("\n");
    stopped = false;
    return true;

}

bool rd::RdMockupImageManager::stop()
{
    RD_DEBUG("\n");
    stopped = true;
    enabled = false;
    return true;
}

bool rd::RdMockupImageManager::isStopped()
{
    return stopped;
}

bool rd::RdMockupImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::RdMockupImageManager::isEnabled()
{
    return enabled;
}

bool rd::RdMockupImageManager::configure(std::string parameter, std::string value)
{
    RD_DEBUG("Configure called for parameter: \"%s\" with value: \"%s\"\n", parameter.c_str(), value.c_str());
    return RdImageManager::configure(parameter, value);
}

rd::RdImage rd::RdMockupImageManager::getImage()
{
    semaphore.wait();
    RdImage return_image(image);
    semaphore.post();

    return return_image;
}

bool rd::RdMockupImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdMockupImageManager();
    }

    return Register(uniqueInstance, id);
}

rd::RdMockupImageManager::~RdMockupImageManager()
{
    uniqueInstance = NULL;
}

bool rd::RdMockupImageManager::receiveImage(rd::RdImage received_image)
{
    semaphore.wait();
    image = received_image;
    semaphore.post();

    //-- Notify listeners
    if (enabled)
        for (int i = 0; i < listeners.size(); i++)
            listeners[i]->onImageArrived(this);
    else
    {
        RD_WARNING("MockupImageManager is disabled!\n");
        return false;
    }

    return true;
}

rd::RdMockupImageManager::RdMockupImageManager()
{
    stopped = true;
    enabled = false;
}

#include "MockImageManager.hpp"


//-- Initialize static members
rd::MockupImageManager * rd::MockupImageManager::uniqueInstance = NULL;
const std::string rd::MockupImageManager::id = "MOCKUP";


bool rd::MockupImageManager::start()
{
    RD_DEBUG("\n");
    stopped = false;
    return true;

}

bool rd::MockupImageManager::stop()
{
    RD_DEBUG("\n");
    stopped = true;
    enabled = false;
    return true;
}

bool rd::MockupImageManager::isStopped()
{
    return stopped;
}

bool rd::MockupImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::MockupImageManager::isEnabled()
{
    return enabled;
}

bool rd::MockupImageManager::configure(std::string parameter, std::string value)
{
    RD_DEBUG("Configure called for parameter: \"%s\" with value: \"%s\"\n", parameter.c_str(), value.c_str());
    return ImageManager::configure(parameter, value);
}

rd::Image rd::MockupImageManager::getImage()
{
    semaphore.wait();
    Image return_image(image);
    semaphore.post();

    return return_image;
}

bool rd::MockupImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockupImageManager();
    }

    return Register(uniqueInstance, id);
}

rd::MockupImageManager::~MockupImageManager()
{
    uniqueInstance = NULL;
}

bool rd::MockupImageManager::receiveImage(rd::Image received_image)
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

rd::MockupImageManager::MockupImageManager()
{
    stopped = true;
    enabled = false;
}

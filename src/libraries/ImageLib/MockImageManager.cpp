#include "MockImageManager.hpp"


//-- Initialize static members
rd::MockImageManager * rd::MockImageManager::uniqueInstance = NULL;
const std::string rd::MockImageManager::id = "MOCKUP";


bool rd::MockImageManager::start()
{
    RD_DEBUG("\n");
    stopped = false;
    return true;

}

bool rd::MockImageManager::stop()
{
    RD_DEBUG("\n");
    stopped = true;
    enabled = false;
    return true;
}

bool rd::MockImageManager::isStopped()
{
    return stopped;
}

bool rd::MockImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::MockImageManager::isEnabled()
{
    return enabled;
}

bool rd::MockImageManager::configure(std::string parameter, std::string value)
{
    RD_DEBUG("Configure called for parameter: \"%s\" with value: \"%s\"\n", parameter.c_str(), value.c_str());
    return ImageManager::configure(parameter, value);
}

rd::Image rd::MockImageManager::getImage()
{
    semaphore.wait();
    Image return_image(image);
    semaphore.post();

    return return_image;
}

bool rd::MockImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new MockImageManager();
    }

    return Register(uniqueInstance, id);
}

rd::MockImageManager::~MockImageManager()
{
    uniqueInstance = NULL;
}

bool rd::MockImageManager::receiveImage(rd::Image received_image)
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
        RD_WARNING("MockImageManager is disabled!\n");
        return false;
    }

    return true;
}

rd::MockImageManager::MockImageManager()
{
    stopped = true;
    enabled = false;
}

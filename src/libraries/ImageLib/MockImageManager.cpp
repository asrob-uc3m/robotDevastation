// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockImageManager.hpp"

#include <ColorDebug.hpp>

//-- Initialize static members
rd::MockImageManager * rd::MockImageManager::uniqueInstance = NULL;
const std::string rd::MockImageManager::id = "MOCK";


bool rd::MockImageManager::start()
{
    CD_DEBUG("\n");
    stopped = false;
    return true;

}

bool rd::MockImageManager::stop()
{
    CD_DEBUG("\n");
    stopped = true;
    enabled = false;
    return true;
}

bool rd::MockImageManager::isStopped() const
{
    return stopped;
}

bool rd::MockImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::MockImageManager::isEnabled() const
{
    return enabled;
}

bool rd::MockImageManager::configure(const std::string & parameter, const std::string & value)
{
    CD_DEBUG("Configure called for parameter: \"%s\" with value: \"%s\"\n", parameter.c_str(), value.c_str());
    return ImageManager::configure(parameter, value);
}

rd::Image rd::MockImageManager::getImage() const
{
    semaphore.wait();
    Image return_image = image;
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

bool rd::MockImageManager::receiveImage(const Image & received_image)
{
    semaphore.wait();
    image = received_image;
    semaphore.post();

    //-- Notify listeners
    if (enabled)
    {
        for (std::vector<ImageEventListener *>::iterator it = listeners.begin(); it != listeners.end(); ++it)
        {
            (*it)->onImageArrived(this);
        }
    }
    else
    {
        CD_WARNING("MockImageManager is disabled!\n");
        return false;
    }

    return true;
}

rd::MockImageManager::MockImageManager()
{
    stopped = true;
    enabled = false;
}

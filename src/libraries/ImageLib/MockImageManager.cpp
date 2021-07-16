// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockImageManager.hpp"

#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"

//-- Initialize static members
rd::MockImageManager * rd::MockImageManager::uniqueInstance = NULL;
const std::string rd::MockImageManager::id = "MOCK";

bool rd::MockImageManager::start()
{
    stopped = false;
    return true;

}

bool rd::MockImageManager::stop()
{
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
    yCDebug(RD_IMG) << "Configure called for parameter" << parameter << "with value" << value;
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
        yCWarning(RD_IMG) << "MockImageManager is disabled!";
        return false;
    }

    return true;
}

rd::MockImageManager::MockImageManager()
{
    stopped = true;
    enabled = false;
}

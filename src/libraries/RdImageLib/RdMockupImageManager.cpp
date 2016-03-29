#include "RdMockupImageManager.hpp"


//-- Initialize static members
rd::RdMockupImageManager * rd::RdMockupImageManager::uniqueInstance = NULL;
const std::string rd::RdMockupImageManager::id = "MOCKUP";


bool rd::RdMockupImageManager::start()
{
    RD_DEBUG("\n");
    image.resize(640,480);
    yarp::sig::PixelRgb blue(0,0,255);
    yarp::sig::draw::addCircle(image,blue,320,240,10);
    stopped = false;
    return true;

}

bool rd::RdMockupImageManager::stop()
{
    RD_DEBUG("\n");
    stopped = true;
    return true;
}

bool rd::RdMockupImageManager::isStopped()
{
    return stopped;
}

bool rd::RdMockupImageManager::configure(std::string parameter, std::string value)
{
    RD_DEBUG("\n");
    return RdImageManager::configure(parameter, value);
}

rd::RdImage rd::RdMockupImageManager::getImage()
{
    return image;
}

bool rd::RdMockupImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdMockupImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdMockupImageManager::~RdMockupImageManager()
{
    uniqueInstance = NULL;
}

bool rd::RdMockupImageManager::receiveImage(rd::RdImage received_image)
{
    return false;
}

void rd::RdMockupImageManager::run()
{
    //-- Notify listeners
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners[i]->onImageArrived(this);
    }
}

rd::RdMockupImageManager::RdMockupImageManager()
{
    stopped = true;
}

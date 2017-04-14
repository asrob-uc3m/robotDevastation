#include "YarpLocalImageManager.hpp"

#include <sstream>

#include <yarp/os/Network.h>
#include <yarp/os/Property.h>

#include "Macros.hpp"

//-- Initialize static members
rd::YarpLocalImageManager * rd::YarpLocalImageManager::uniqueInstance = NULL;
const std::string rd::YarpLocalImageManager::id = "YARPlocal";


bool rd::YarpLocalImageManager::start()
{
    yarp::os::Network::init();

    yarp::os::Property cameraOptions;
    cameraOptions.put("device","grabber");
    cameraOptions.put("subdevice","opencv_grabber");
    cameraOptions.put("name",remote_port_name);
    RD_DEBUG("Camera id: %d\n", camera_id);
    cameraOptions.put("camera", camera_id);
    cameraDevice.open(cameraOptions);
    if(!cameraDevice.isValid())
    {
        RD_ERROR("Could not open opencv_grabber device.\n");
        return false;
    }
    RD_SUCCESS("Opened opencv_grabber device.\n");

    imagePort.open(local_port_name.c_str());
    imagePort.useCallback(*this);

    if(! yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str(), "mjpeg" ) )
    {
        RD_WARNING("Could not connect to robot camera via mjpeg.\n");
        if (!yarp::os::Network::connect(remote_port_name.c_str(), local_port_name.c_str()))
        {
            RD_WARNING("Could not connect to robot camera.\n");
            return false;
        }
    }

    RD_SUCCESS("Connected to robot camera.\n");

    stopped = false;
    enabled = false;
    return true;

}

bool rd::YarpLocalImageManager::stop()
{
    imagePort.disableCallback();
    imagePort.interrupt();
    imagePort.close();

    cameraDevice.close();

    stopped = true;
    enabled = false;

    return true;
}

bool rd::YarpLocalImageManager::isStopped() const
{
    return stopped;
}

bool rd::YarpLocalImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::YarpLocalImageManager::configure(const std::string & parameter, const std::string & value)
{
    if ( parameter.compare("remote_img_port") == 0 && value.compare("") != 0)
    {
        remote_port_name = value;
        return true;
    }
    else if ( parameter.compare("local_img_port") == 0 && value.compare("") != 0)
    {
        local_port_name = value;
        return true;
    }
    else if (parameter.compare("camera_id") == 0)
    {
        std::stringstream ss(value);
        ss >> camera_id;
        return true;
    }
    else
        return ImageManager::configure(parameter, value);
}

rd::Image rd::YarpLocalImageManager::getImage() const
{
    semaphore.wait();
    Image return_image = image;
    semaphore.post();

    return return_image;
}

bool rd::YarpLocalImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new YarpLocalImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::YarpLocalImageManager::~YarpLocalImageManager()
{
    uniqueInstance = NULL;
}

void rd::YarpLocalImageManager::onRead(Image &image)
{
    semaphore.wait();
    this->image=image;
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
        RD_WARNING("YarpImageManager is disabled!\n");
    }
}

rd::YarpLocalImageManager::YarpLocalImageManager()
{
    stopped = true;
    enabled = false;
    camera_id = -1;
}

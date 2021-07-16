// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpLocalImageManager.hpp"

#include <sstream>

#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/Property.h>

#include "LogComponent.hpp"

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
    yCDebug(RD_IMG) << "Camera id:" << camera_id;
    cameraOptions.put("camera", camera_id);
    cameraDevice.open(cameraOptions);
    if(!cameraDevice.isValid())
    {
        yCError(RD_IMG) << "Could not open opencv_grabber device";
        return false;
    }
    yCInfo(RD_IMG) << "Opened opencv_grabber device";

    imagePort.open(local_port_name.c_str());
    imagePort.useCallback(*this);

    if(! yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str(), "mjpeg" ) )
    {
        yCWarning(RD_IMG) << "Could not connect to robot camera via mjpeg";
        if (!yarp::os::Network::connect(remote_port_name.c_str(), local_port_name.c_str()))
        {
            yCWarning(RD_IMG) << "Could not connect to robot camera";
            return false;
        }
    }

    yCInfo(RD_IMG) << "Connected to robot camera";

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
        yCWarning(RD_IMG) << "YarpImageManager is disabled!";
    }
}

rd::YarpLocalImageManager::YarpLocalImageManager()
{
    stopped = true;
    enabled = false;
    camera_id = 0; // see OpenCV C API: CV_CAP_ANY
}

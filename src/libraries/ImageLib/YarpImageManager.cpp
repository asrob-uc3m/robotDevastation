// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpImageManager.hpp"

#include <yarp/os/LogStream.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

#include "LogComponent.hpp"

//-- Initialize static members
rd::YarpImageManager * rd::YarpImageManager::uniqueInstance = NULL;
const std::string rd::YarpImageManager::id = "YARP";


bool rd::YarpImageManager::start()
{
    yarp::os::NetworkBase::initMinimum();

    if ( ! yarp::os::NetworkBase::checkNetwork() )
    {
        yCError(RD_IMG) << "Found no yarp network";
        yCInfo(RD_IMG) << "Try running 'yarpserver &'', or '--fakeImageManager' or '--yarpLocalImageManager' for Fake robot camera";
        return false;
    }

    imagePort.open(local_port_name.c_str());

    //-- Connect from robot camera to robotDevastation (via mjpeg)
    int mjpegTries = 0;
    while(mjpegTries++ < 10)
   {
        if(imagePort.getInputCount() > 0)
            break;
        yCInfo(RD_IMG) << "Try to connect from robot camera to robotDevastation (via mjpeg). Attempt:" << mjpegTries;
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str(), "mjpeg" );
    }
    if (mjpegTries == 11)  //-- 11 allows  10 attempts
    {
        yCError(RD_IMG) << "Timeout for connect from robot camera to robotDevastation (via mjpeg)";

        //-- Connect from robot camera to robotDevastation (NON-mjpeg)
        int tries = 0;
        while(tries++ < 10)
       {
            if(imagePort.getInputCount() > 0)
                break;
            yCInfo(RD_IMG) << "Try to connect from robot camera to robotDevastation (NON-mjpeg). Attempt:" << tries;
            yarp::os::Time::delay(0.5);
            yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str());
        }
        if (tries == 11)  //-- 11 allows  10 attempts
        {
            yCError(RD_IMG) << "Timeout for connect from robot camera to robotDevastation (NON-mjpeg)";
            yCError(RD_IMG) << "Could not connect to robot camera";
            yCInfo(RD_IMG) << "If you prefer a fake camera use the '--fakeImageManager' or '--yarpLocalImageManager' parameter to run robotDevastation";
            return false;
        }
        else
        {
            yCInfo(RD_IMG) << "Connection from robot camera to robotDevastation (NON-mjpeg)";
        }
    }
    else
    {
        yCInfo(RD_IMG) << "Connection from robot camera to robotDevastation (via mjpeg)";
    }

    imagePort.useCallback(*this);

    stopped = false;
    enabled = false;
    return true;
}

bool rd::YarpImageManager::stop()
{
    imagePort.disableCallback();
    imagePort.interrupt();
    imagePort.close();

    stopped = true;
    enabled = false;

    return true;
}

bool rd::YarpImageManager::isStopped() const
{
    return stopped;
}

bool rd::YarpImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::YarpImageManager::configure(const std::string & parameter, const std::string & value)
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
    else
        return ImageManager::configure(parameter, value);
}

rd::Image rd::YarpImageManager::getImage() const
{
    semaphore.wait();
    Image return_image = image;
    semaphore.post();

    return return_image;
}

bool rd::YarpImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new YarpImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::YarpImageManager::~YarpImageManager()
{
    uniqueInstance = NULL;
}

void rd::YarpImageManager::onRead(Image &image)
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

rd::YarpImageManager::YarpImageManager()
{
    stopped = true;
    enabled = false;
}

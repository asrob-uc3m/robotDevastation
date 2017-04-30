#include "YarpImageManager.hpp"

#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

#include "Macros.hpp"

//-- Initialize static members
rd::YarpImageManager * rd::YarpImageManager::uniqueInstance = NULL;
const std::string rd::YarpImageManager::id = "YARP";


bool rd::YarpImageManager::start()
{
    yarp::os::NetworkBase::initMinimum();

    if ( ! yarp::os::NetworkBase::checkNetwork() )
    {
        RD_ERROR("Found no yarp network. Bye!\n");
        RD_INFO("try running 'yarpserver &'', or '--mockImageManager' or '--yarpLocalImageManager' for Fake robot camera.\n");
        return false;
    }

    imagePort.open(local_port_name.c_str());

    //-- Connect from robot camera to robotDevastation (via mjpeg)
    int mjpegTries = 0;
    while(mjpegTries++ < 10)
   {
        if(imagePort.getInputCount() > 0)
            break;
        RD_INFO("Try to connect from robot camera to robotDevastation (via mjpeg). Attempt: %d\n",mjpegTries);
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str(), "mjpeg" );
    }
    if (mjpegTries == 11)  //-- 11 allows  10 attempts
    {
        RD_ERROR("Timeout for connect from robot camera to robotDevastation (via mjpeg).\n");

        //-- Connect from robot camera to robotDevastation (NON-mjpeg)
        int tries = 0;
        while(tries++ < 10)
       {
            if(imagePort.getInputCount() > 0)
                break;
            RD_INFO("Try to connect from robot camera to robotDevastation (NON-mjpeg). Attempt: %d\n",tries);
            yarp::os::Time::delay(0.5);
            yarp::os::Network::connect( remote_port_name.c_str(), local_port_name.c_str());
        }
        if (tries == 11)  //-- 11 allows  10 attempts
        {
            RD_ERROR("Timeout for connect from robot camera to robotDevastation (NON-mjpeg).\n");
            RD_ERROR("Could not connect to robot camera.\n");
            RD_INFO("If you prefer a fake camera use the '--mockImageManager' or '--yarpLocalImageManager' parameter to run robotDevastation.\n");
            return false;
        }
        else
        {
            RD_SUCCESS(" Connect from robot camera to robotDevastation (NON-mjpeg).\n");
        }
    }
    else
    {
        RD_SUCCESS(" Connect from robot camera to robotDevastation (via mjpeg).\n");
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
        RD_WARNING("YarpImageManager is disabled!\n");
    }
}

rd::YarpImageManager::YarpImageManager()
{
    stopped = true;
    enabled = false;
}

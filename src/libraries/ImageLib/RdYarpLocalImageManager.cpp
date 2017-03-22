#include "RdYarpLocalImageManager.hpp"


//-- Initialize static members
rd::RdYarpLocalImageManager * rd::RdYarpLocalImageManager::uniqueInstance = NULL;
const std::string rd::RdYarpLocalImageManager::id = "YARPlocal";


bool rd::RdYarpLocalImageManager::start()
{
    yarp::os::Network::init();

    yarp::os::Property cameraOptions;
    cameraOptions.put("device","grabber");
    cameraOptions.put("subdevice","opencv_grabber");
    cameraOptions.put("name",remote_port_name);
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

bool rd::RdYarpLocalImageManager::stop()
{
    imagePort.disableCallback();
    imagePort.interrupt();
    imagePort.close();

    cameraDevice.close();

    stopped = true;
    enabled = false;

    return true;
}

bool rd::RdYarpLocalImageManager::isStopped()
{
    return stopped;
}

bool rd::RdYarpLocalImageManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    return true;
}

bool rd::RdYarpLocalImageManager::configure(std::string parameter, std::string value)
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
        return RdImageManager::configure(parameter, value);
}

rd::RdImage rd::RdYarpLocalImageManager::getImage()
{
    semaphore.wait();
    RdImage return_image(image);
    semaphore.post();

    return return_image;
}

bool rd::RdYarpLocalImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdYarpLocalImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdYarpLocalImageManager::~RdYarpLocalImageManager()
{
    uniqueInstance = NULL;
}

void rd::RdYarpLocalImageManager::onRead(rd::RdImage &image)
{
    semaphore.wait();
    this->image=image;
    semaphore.post();

    //-- Notify listeners
    if (enabled)
        for (int i = 0; i < listeners.size(); i++)
            listeners[i]->onImageArrived(this);
    else
    {
        RD_WARNING("YarpImageManager is disabled!\n");
    }
}

rd::RdYarpLocalImageManager::RdYarpLocalImageManager()
{
    stopped = true;
    enabled = false;
}

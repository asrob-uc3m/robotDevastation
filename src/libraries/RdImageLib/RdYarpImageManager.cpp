#include "RdYarpImageManager.hpp"


//-- Initialize static members
rd::RdYarpImageManager * rd::RdYarpImageManager::uniqueInstance = NULL;
const std::string rd::RdYarpImageManager::id = "YARP";


bool rd::RdYarpImageManager::start()
{
    yarp::os::Network::init();

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
    return true;

}

bool rd::RdYarpImageManager::stop()
{
    imagePort.disableCallback();
    imagePort.interrupt();
    imagePort.close();

    return true;
}

bool rd::RdYarpImageManager::configure(std::string parameter, std::string value)
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

rd::RdImage rd::RdYarpImageManager::getImage()
{
    semaphore.wait();
    RdImage return_image(image);
    semaphore.post();

    return return_image;
}

bool rd::RdYarpImageManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdYarpImageManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdYarpImageManager::~RdYarpImageManager()
{
    uniqueInstance = NULL;
}

void rd::RdYarpImageManager::onRead(rd::RdImage &image)
{
    semaphore.wait();
    this->image=image;
    semaphore.post();

    //-- Notify listeners
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners[i]->onImageArrived(this);
    }
}

rd::RdYarpImageManager::RdYarpImageManager()
{

}
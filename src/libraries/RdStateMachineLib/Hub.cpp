#include "Hub.hpp"


bool rd::ManagerHub::setNetworkManager(rd::RdNetworkManager *networkManager)
{
    this->networkManager = networkManager;
}

bool rd::ManagerHub::setImageManager(rd::RdImageManager *imageManager)
{
    this->imageManager = imageManager;
}

bool rd::ManagerHub::setInputManager(rd::RdInputManager *inputManager)
{
    this->inputManager = inputManager;
}

bool rd::ManagerHub::setMentalMap(rd::RdMentalMap *mentalMap)
{
    this->mentalMap = mentalMap;
}

bool rd::ManagerHub::setRobotManager(rd::RdRobotManager *robotManager)
{
    this->robotManager = robotManager;
}

#include "Hub.hpp"

rd::ManagerHub::ManagerHub()
{
    networkManager = NULL;
    imageManager = NULL;
    inputManager = NULL;
    mentalMap = NULL;
    robotManager = NULL;
}

rd::ManagerHub::ManagerHub(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                           rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                           rd::RdRobotManager *robotManager, rd::AudioManager *audioManager)
{
    this->networkManager = networkManager;
    this->imageManager = imageManager;
    this->inputManager = inputManager;
    this->mentalMap = mentalMap;
    this->robotManager = robotManager;
    this->audioManager = audioManager;
}

bool rd::ManagerHub::setNetworkManager(rd::RdNetworkManager *networkManager) { this->networkManager = networkManager;}
bool rd::ManagerHub::setImageManager(rd::RdImageManager *imageManager) { this->imageManager = imageManager;}
bool rd::ManagerHub::setInputManager(rd::RdInputManager *inputManager) { this->inputManager = inputManager;}
bool rd::ManagerHub::setMentalMap(rd::RdMentalMap *mentalMap) { this->mentalMap = mentalMap;}
bool rd::ManagerHub::setRobotManager(rd::RdRobotManager *robotManager) { this->robotManager = robotManager;}
bool rd::ManagerHub::setAudioManager(rd::AudioManager *audioManager) { this->audioManager = audioManager;}

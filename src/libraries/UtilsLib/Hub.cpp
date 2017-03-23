#include "Hub.hpp"

rd::ManagerHub::ManagerHub()
{
    networkManager = NULL;
    imageManager = NULL;
    inputManager = NULL;
    mentalMap = NULL;
    robotManager = NULL;
}

rd::ManagerHub::ManagerHub(rd::NetworkManager *networkManager, rd::ImageManager *imageManager,
                           rd::InputManager *inputManager, rd::MentalMap *mentalMap,
                           rd::RobotManager *robotManager, rd::AudioManager *audioManager,
                           ScreenManager *screenManager)
{
    this->networkManager = networkManager;
    this->imageManager = imageManager;
    this->inputManager = inputManager;
    this->mentalMap = mentalMap;
    this->robotManager = robotManager;
    this->audioManager = audioManager;
    this->screenManager = screenManager;
}

bool rd::ManagerHub::setNetworkManager(rd::NetworkManager *networkManager) { this->networkManager = networkManager; return true; }
bool rd::ManagerHub::setImageManager(rd::ImageManager *imageManager) { this->imageManager = imageManager; return true; }
bool rd::ManagerHub::setInputManager(rd::InputManager *inputManager) { this->inputManager = inputManager; return true; }
bool rd::ManagerHub::setMentalMap(rd::MentalMap *mentalMap) { this->mentalMap = mentalMap; return true; }
bool rd::ManagerHub::setRobotManager(rd::RobotManager *robotManager) { this->robotManager = robotManager; return true; }
bool rd::ManagerHub::setAudioManager(rd::AudioManager *audioManager) { this->audioManager = audioManager; return true; }
bool rd::ManagerHub::setScreenManager(rd::ScreenManager *screenManager) { this->screenManager = screenManager; return true; }

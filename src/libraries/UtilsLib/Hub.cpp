#include "Hub.hpp"

rd::ManagerHub::ManagerHub()
{
    networkManager = NULL;
    imageManager = NULL;
    inputManager = NULL;
    mentalMap = NULL;
    robotManager = NULL;
    audioManager = NULL;
    screenManager = NULL;
}

rd::ManagerHub::ManagerHub(NetworkManager *networkManager, ImageManager *imageManager,
                           InputManager *inputManager, MentalMap *mentalMap,
                           RobotManager *robotManager, AudioManager *audioManager,
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

bool rd::ManagerHub::setNetworkManager(NetworkManager *networkManager) { this->networkManager = networkManager; return true; }
bool rd::ManagerHub::setImageManager(ImageManager *imageManager) { this->imageManager = imageManager; return true; }
bool rd::ManagerHub::setInputManager(InputManager *inputManager) { this->inputManager = inputManager; return true; }
bool rd::ManagerHub::setMentalMap(MentalMap *mentalMap) { this->mentalMap = mentalMap; return true; }
bool rd::ManagerHub::setRobotManager(RobotManager *robotManager) { this->robotManager = robotManager; return true; }
bool rd::ManagerHub::setAudioManager(AudioManager *audioManager) { this->audioManager = audioManager; return true; }
bool rd::ManagerHub::setScreenManager(ScreenManager *screenManager) { this->screenManager = screenManager; return true; }

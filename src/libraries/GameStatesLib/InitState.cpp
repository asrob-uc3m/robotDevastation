#include "InitState.hpp"

const int rd::InitState::LOGIN_SUCCESSFUL = 1;

rd::InitState::InitState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager) :
    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
{
    login = false;
    logged_in = false;
}

rd::InitState::~InitState()
{

}

bool rd::InitState::setup()
{
    //-- Show Robot Devastation start screen:
    screen.show();

    //-- Start Robot Devastation music theme:
    audioManager->start();
    audioManager->play("RD_THEME", -1);

    //-- Wait for input (set listener):
    inputManager->addInputEventListener(this);
    inputManager->start();

    //-- Setup network manager
    networkManager->addNetworkEventListener(mentalMap);
    mentalMap->addMentalMapEventListener((RdYarpNetworkManager *)networkManager);
    networkManager->start();

    return true;
}

bool rd::InitState::loop()
{
    if (login)
    {
        //-- Log in
        networkManager->login(mentalMap->getMyself());
        logged_in = true;
    }

    return true;
}

bool rd::InitState::cleanup()
{
    RD_DEBUG("Cleanup!!\n");
    audioManager->stopMusic();
    inputManager->removeInputEventListeners();

    return true;
}

int rd::InitState::evaluateConditions()
{
    if (logged_in)
        return LOGIN_SUCCESSFUL;

    return -1;
}

bool rd::InitState::onKeyDown(rd::RdKey k)
{
    return true;
}

bool rd::InitState::onKeyUp(rd::RdKey k)
{
    if (k.getValue() == RdKey::KEY_ENTER)
        RD_DEBUG("Enter was pressed!\n");
        login = true;
}
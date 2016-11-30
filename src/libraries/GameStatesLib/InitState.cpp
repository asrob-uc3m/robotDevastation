#include "InitState.hpp"

const int rd::InitState::LOGIN_SUCCESSFUL = 1;

rd::InitState::InitState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager,
                         rd::ScreenManager *screenManager) :
    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager, screenManager)
{
    state_id = "InitState";
    login = false;
    logged_in = false;
}

rd::InitState::~InitState()
{

}

bool rd::InitState::setup()
{
    //-- Show Robot Devastation start screen:
    if( ! screen.init() )
        return false;
    screenManager->setCurrentScreen(&screen);

    //-- Start Robot Devastation music theme:
    audioManager->start();
    audioManager->play("RD_THEME", -1);

    //-- Wait for input (set listener):
    inputManager->addInputEventListener(this);
    inputManager->start();

    //-- Setup network manager
    networkManager->addNetworkEventListener(mentalMap);
    mentalMap->addMentalMapEventListener((RdYarpNetworkManager *)networkManager);
    if( ! networkManager->start() )
        return false;

    return true;
}

bool rd::InitState::loop()
{
    //-- Get input events
    inputManager->refreshEvents();

    //-- Do stuff
    if (login)
    {
        //-- Log in
        networkManager->login();
        if( ! robotManager->connect() )
            return false;
        robotManager->setEnabled(false);
        if( ! imageManager->start() )
            return false;
        logged_in = true;
    }

    //-- Show graphics
    if(!screenManager->show())
        return false;

    return true;
}

bool rd::InitState::cleanup()
{
    RD_DEBUG("Cleanup!!\n");
    audioManager->stopMusic();
    inputManager->removeInputEventListeners();
    screen.cleanup();

    return true;
}

int rd::InitState::evaluateConditions()
{
    if (logged_in)
        return LOGIN_SUCCESSFUL;

    return -1;
}

bool rd::InitState::onKeyDown(const rd::RdKey & k)
{
    return true;
}

bool rd::InitState::onKeyUp(const rd::RdKey & k)
{
    if (k.isControlKey() || k.isPrintable())
    {
        RD_DEBUG("Key was pressed!\n");
        login = true;
    }
}

bool rd::InitState::onWindowEvent(const rd::RdWindowEvent & event)
{
    if (event.getEvent() == SDL_WINDOWEVENT_CLOSE)
    {
        RD_DEBUG("Not yet implemented!\n");
        return true;
    }
    return false;
}

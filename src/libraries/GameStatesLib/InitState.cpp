#include "InitState.hpp"

const int rd::InitState::LOGIN_SUCCESSFUL = 1;
const int rd::InitState::EXIT_REQUESTED = 2;

rd::InitState::InitState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager,
                         rd::ScreenManager *screenManager) :
    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager, screenManager)
{
    state_id = "InitState";
    login = false;
    logged_in = false;
    received_exit = false;
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

    if (received_exit)
    {
        //-- Stop things to exit game (logout)
        inputManager->stop();
        audioManager->stop();
        networkManager->stop();
        robotManager->setEnabled(false);
    }

    return true;
}

int rd::InitState::evaluateConditions()
{
    if (logged_in)
        return LOGIN_SUCCESSFUL;

    if (received_exit)
        return EXIT_REQUESTED;

    return -1;
}

bool rd::InitState::onKeyDown(const rd::RdKey & k)
{
    return true;
}

bool rd::InitState::onKeyUp(const rd::RdKey & k)
{
    if (k.getValue() == rd::RdKey::KEY_ESCAPE)
    {
        RD_DEBUG("Escape was pressed!\n");
        received_exit = true;
        return true;
    }
    else if (k.isControlKey() || k.isPrintable())
    {
        RD_DEBUG("Key was pressed!\n");
        login = true;
        return true;
    }

    return false;
}

bool rd::InitState::onWindowEvent(const rd::RdWindowEvent & event)
{
    if (event.getEvent() == rd::RdWindowEvent::WINDOW_CLOSE)
    {
        RD_DEBUG("Exit was triggered!\n");
        received_exit = true;
        return true;
    }
    return false;
}

// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "InitState.hpp"
#include "YarpNetworkManager.hpp"
#include "Macros.hpp"

const int rd::InitState::LOGIN_SUCCESSFUL = 1;
const int rd::InitState::EXIT_REQUESTED = 2;

rd::InitState::InitState(NetworkManager *networkManager, ImageManager *imageManager,
                         InputManager *inputManager, MentalMap *mentalMap,
                         RobotManager *robotManager, AudioManager *audioManager,
                         ScreenManager *screenManager) :
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
    mentalMap->addMentalMapEventListener((YarpNetworkManager *)networkManager);
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
        if( ! robotManager->connect() )
            return false;
        robotManager->setEnabled(false);
        if( ! imageManager->start() )
            return false;
        //-- Log in
        if( ! networkManager->login() )
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

bool rd::InitState::onKeyDown(const Key & k)
{
    return true;
}

bool rd::InitState::onKeyUp(const Key & k)
{
    if (k.getValue() == Key::KEY_ESCAPE)
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

bool rd::InitState::onWindowEvent(const WindowEvent & event)
{
    if (event.getEvent() == WindowEvent::WINDOW_CLOSE)
    {
        RD_DEBUG("Exit was triggered!\n");
        received_exit = true;
        return true;
    }
    return false;
}

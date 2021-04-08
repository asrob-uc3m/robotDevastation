// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "GameState.hpp"

#include <yarp/os/LogStream.h>

const int rd::GameState::KILLED = 1;
const int rd::GameState::EXIT_REQUESTED = 2;

const rd::Key rd::GameState::KEY_EXIT = rd::Key::KEY_ESCAPE;
const rd::Key rd::GameState::KEY_SHOOT = rd::Key::KEY_SPACE;
const rd::Key rd::GameState::KEY_RELOAD = 'r';
const rd::Key rd::GameState::KEY_MOVE_FWD = 'w';
const rd::Key rd::GameState::KEY_MOVE_BACK = 's';
const rd::Key rd::GameState::KEY_TURN_LEFT = 'a';
const rd::Key rd::GameState::KEY_TURN_RIGHT = 'd';
const rd::Key rd::GameState::KEY_PAN_LEFT = rd::Key::KEY_ARROW_LEFT;
const rd::Key rd::GameState::KEY_PAN_RIGHT = rd::Key::KEY_ARROW_RIGHT;
const rd::Key rd::GameState::KEY_TILT_UP = rd::Key::KEY_ARROW_UP;
const rd::Key rd::GameState::KEY_TILT_DOWN = rd::Key::KEY_ARROW_DOWN;

rd::GameState::GameState(NetworkManager *networkManager, ImageManager *imageManager,
                         InputManager *inputManager, MentalMap *mentalMap,
                         asrob::IRobotManager *robotManager, AudioManager *audioManager,
                         ScreenManager *screenManager) :
                    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager,
                               audioManager, screenManager)
{
    state_id = "GameState";
    received_exit = false;
}

rd::GameState::~GameState()
{

}

bool rd::GameState::setup()
{
    //-- Connect mentalMap to networkManager to receive updates (and vice-versa)
    if (mentalMap!=NULL && networkManager!=NULL)
    {
        networkManager->addNetworkEventListener(mentalMap);
        mentalMap->addMentalMapEventListener(networkManager);
    }
    else
    {
        yError() << "Null mentalMap or networkManager";
        return false;
    }

    //-- Start video
    if (imageManager!=NULL)
    {
        imageManager->setEnabled(true);
        imageManager->addImageEventListener(&processorImageEventListener);
        imageManager->addImageEventListener(this);
    }
    else
    {
        yError() << "Null imageManager";
        return false;
    }

    //-- Start audio
    if (audioManager!=NULL)
    {
        audioManager->start();
        audioManager->play("RD_THEME", -1);
    }
    else
    {
        yError() << "Null audioManager";
        return false;
    }

    //-- Enable input
    if (inputManager!=NULL)
    {
        inputManager->addInputEventListener(this);
    }
    else
    {
        yError() << "Null inputManager";
        return false;
    }

    //-- Show Robot Devastation game screen:
    if (!screen.init())
    {
        yError() << "Could not init game screen";
        return false;
    }

    //-- Set info elements on GameScreen
    screenManager->setCurrentScreen(&screen);
    screenManager->update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screenManager->update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screenManager->update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screenManager->update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());

    return true;
}

bool rd::GameState::loop()
{
    //-- Get input events
    inputManager->refreshEvents();

    //-- Set info elements on GameScreen
    screenManager->update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screenManager->update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screenManager->update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screenManager->update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());

    //-- Show graphics
    if(!screenManager->show())
    {
        yError() << "Could not show game screen";
        return false;
    }
    return true;
}

bool rd::GameState::cleanup()
{
    screen.cleanup();

    if (received_exit)
    {
        //-- Stop things to exit game (logout)
        imageManager->stop();
        inputManager->stop();
        inputManager->removeInputEventListeners();
        audioManager->stopMusic();
        audioManager->stop();
        networkManager->logout();
        networkManager->stop();
        return true;
    }
    else
        return false;

}

int rd::GameState::evaluateConditions()
{
    if (mentalMap->getMyself().getHealth()==0)
        return KILLED;

    if (received_exit)
        return EXIT_REQUESTED;

    return -1;
}

bool rd::GameState::onKeyDown(const Key & k)
{
    if (k == KEY_SHOOT)
    {
        mentalMap->shoot();
        return true;
    }

    if (k == KEY_RELOAD)
    {
        mentalMap->reload();
        return true;
    }

    if (k == KEY_EXIT)
    {
        yDebug() << "Exit was triggered!";
        received_exit = true;
        return true;
    }

    //-- Movement control
    if (k == KEY_TURN_LEFT)
    {
        yDebug() << "Turn left was triggered!";
        robotManager->turnLeft(1.0);
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        yDebug() << "Turn right was triggered!";
        robotManager->turnLeft(-1.0);
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        yDebug() << "Move forward was triggered!";
        robotManager->moveForward(1.0);
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        yDebug() << "Move backwards was triggered!";
        robotManager->moveForward(-1.0);
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        yDebug() << "Pan left was triggered!";
        robotManager->panLeft(30.0);
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        yDebug() << "Pan right was triggered!";
        robotManager->panLeft(-30.0);
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        yDebug() << "Tilt pos was triggered!";
        robotManager->tiltDown(-30.0);
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        yDebug() << "Tilt neg was triggered!";
        robotManager->tiltDown(30.0);
        return true;
    }

    return false;
}

bool rd::GameState::onKeyUp(const Key & k)
{
    //-- Movement control
    if (k == KEY_TURN_LEFT)
    {
        yDebug() << "Turn left was released!";
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        yDebug() << "Turn right was released!";
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        yDebug() << "Move forward was released!";
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        yDebug() << "Move backwards was released!";
        robotManager->stopMovement();
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        yDebug() << "Pan left was released!";
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        yDebug() << "Pan right was released!";
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        yDebug() << "Tilt up was released!";
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        yDebug() << "Tilt down was released!";
        robotManager->stopCameraMovement();
        return true;
    }
    return false;
}

bool rd::GameState::onWindowEvent(const WindowEvent & event)
{
    if (event.getEvent() == WindowEvent::WINDOW_CLOSE)
    {
        yDebug() << "Exit was triggered!";
        received_exit = true;
        return true;
    }
    return false;
}

bool rd::GameState::onImageArrived(ImageManager *manager)
{
    //-- Don't know if this is safe enough or some mutex is required
    screenManager->update(GameScreen::PARAM_CAMERA_FRAME, manager->getImage());
    return true;
}

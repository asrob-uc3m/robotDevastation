// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "GameState.hpp"

#include <ColorDebug.h>

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
        CD_ERROR("Null mentalMap or networkManager\n");
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
        CD_ERROR("Null imageManager\n");
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
        CD_ERROR("Null audioManager\n");
        return false;
    }

    //-- Enable input
    if (inputManager!=NULL)
    {
        inputManager->addInputEventListener(this);
    }
    else
    {
        CD_ERROR("Null inputManager\n");
        return false;
    }

    //-- Show Robot Devastation game screen:
    if (!screen.init())
    {
        CD_ERROR("Could not init game screen\n");
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
        CD_ERROR("Could not show game screen\n");
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
        CD_DEBUG("Exit was triggered!\n");
        received_exit = true;
        return true;
    }

    //-- Movement control
    if (k == KEY_TURN_LEFT)
    {
        CD_DEBUG("Turn left was triggered!\n");
        robotManager->turnLeft(1.0);
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        CD_DEBUG("Turn right was triggered!\n");
        robotManager->turnLeft(-1.0);
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        CD_DEBUG("Move forward was triggered!\n");
        robotManager->moveForward(1.0);
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        CD_DEBUG("Move backwards was triggered!\n");
        robotManager->moveForward(-1.0);
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        CD_DEBUG("Pan left was triggered!\n");
        robotManager->panLeft(30.0);
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        CD_DEBUG("Pan right was triggered!\n");
        robotManager->panLeft(-30.0);
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        CD_DEBUG("Tilt pos was triggered!\n");
        robotManager->tiltDown(-30.0);
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        CD_DEBUG("Tilt neg was triggered!\n");
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
        CD_DEBUG("Turn left was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        CD_DEBUG("Turn right was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        CD_DEBUG("Move forward was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        CD_DEBUG("Move backwards was released!\n");
        robotManager->stopMovement();
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        CD_DEBUG("Pan left was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        CD_DEBUG("Pan right was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        CD_DEBUG("Tilt up was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        CD_DEBUG("Tilt down was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    return false;
}

bool rd::GameState::onWindowEvent(const WindowEvent & event)
{
    if (event.getEvent() == WindowEvent::WINDOW_CLOSE)
    {
        CD_DEBUG("Exit was triggered!\n");
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

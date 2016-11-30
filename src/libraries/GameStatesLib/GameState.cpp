#include "GameState.hpp"

const int rd::GameState::KILLED = 1;
const int rd::GameState::EXIT_REQUESTED = 2;

const rd::RdKey rd::GameState::KEY_EXIT = rd::MockupKey(rd::RdKey::KEY_ESCAPE);
const rd::RdKey rd::GameState::KEY_SHOOT = rd::MockupKey(rd::RdKey::KEY_SPACE);
const rd::RdKey rd::GameState::KEY_RELOAD = rd::MockupKey('r');
const rd::RdKey rd::GameState::KEY_MOVE_FWD = rd::MockupKey('w');
const rd::RdKey rd::GameState::KEY_MOVE_BACK = rd::MockupKey('s');
const rd::RdKey rd::GameState::KEY_TURN_LEFT = rd::MockupKey('a');
const rd::RdKey rd::GameState::KEY_TURN_RIGHT = rd::MockupKey('d');
const rd::RdKey rd::GameState::KEY_PAN_LEFT = rd::MockupKey(rd::RdKey::KEY_ARROW_LEFT);
const rd::RdKey rd::GameState::KEY_PAN_RIGHT = rd::MockupKey(rd::RdKey::KEY_ARROW_RIGHT);
const rd::RdKey rd::GameState::KEY_TILT_UP = rd::MockupKey(rd::RdKey::KEY_ARROW_UP);
const rd::RdKey rd::GameState::KEY_TILT_DOWN = rd::MockupKey(rd::RdKey::KEY_ARROW_DOWN);

rd::GameState::GameState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, rd::AudioManager *audioManager,
                         rd::ScreenManager *screenManager) :
                    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager,
                               audioManager, screenManager)
{
    this->state_id = "GameState";
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
        RD_ERROR("Null mentalMap or networkManager\n");
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
        RD_ERROR("Null imageManager\n");
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
        RD_ERROR("Null audioManager\n");
        return false;
    }

    //-- Enable input
    if (inputManager!=NULL)
    {
        inputManager->addInputEventListener(this);
    }
    else
    {
        RD_ERROR("Null inputManager\n");
        return false;
    }

    //-- Robot Startup
    robotManager->setEnabled(true);

    //-- Show Robot Devastation game screen:
    if (!screen.init())
    {
        RD_ERROR("Could not init game screen\n");
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
        RD_ERROR("Could not show game screen\n");
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
        robotManager->setEnabled(false);
        robotManager->disconnect();
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

bool rd::GameState::onKeyDown(const rd::RdKey & k)
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
        RD_DEBUG("Exit was triggered!\n");
        received_exit = true;
        return true;
    }

    //-- Movement control
    if (k == KEY_TURN_LEFT)
    {
        RD_DEBUG("Turn left was triggered!\n");
        robotManager->turnLeft();
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        RD_DEBUG("Turn right was triggered!\n");
        robotManager->turnRight();
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        RD_DEBUG("Move forward was triggered!\n");
        robotManager->moveForward();
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        RD_DEBUG("Move backwards was triggered!\n");
        robotManager->moveBackwards();
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        RD_DEBUG("Pan left was triggered!\n");
        robotManager->panLeft();
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        RD_DEBUG("Pan right was triggered!\n");
        robotManager->panRight();
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        RD_DEBUG("Tilt pos was triggered!\n");
        robotManager->tiltUp();
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        RD_DEBUG("Tilt neg was triggered!\n");
        robotManager->tiltDown();
        return true;
    }

    return false;
}

bool rd::GameState::onKeyUp(const rd::RdKey & k)
{
    //-- Movement control
    if (k == KEY_TURN_LEFT)
    {
        RD_DEBUG("Turn left was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_TURN_RIGHT)
    {
        RD_DEBUG("Turn right was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_FWD)
    {
        RD_DEBUG("Move forward was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k == KEY_MOVE_BACK)
    {
        RD_DEBUG("Move backwards was released!\n");
        robotManager->stopMovement();
        return true;
    }

    //-- Pan-tilt control
    if (k == KEY_PAN_LEFT)
    {
        RD_DEBUG("Pan left was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_PAN_RIGHT)
    {
        RD_DEBUG("Pan right was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_UP)
    {
        RD_DEBUG("Tilt up was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    if (k == KEY_TILT_DOWN)
    {
        RD_DEBUG("Tilt down was released!\n");
        robotManager->stopCameraMovement();
        return true;
    }
    return false;
}

bool rd::GameState::onImageArrived(rd::RdImageManager *manager)
{
    //-- Don't know if this is safe enough or some mutex is required
    screenManager->update(GameScreen::PARAM_CAMERA_FRAME, manager->getImage());
}

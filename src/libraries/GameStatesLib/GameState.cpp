#include "GameState.hpp"

const int rd::GameState::KILLED = 1;
const int rd::GameState::EXIT_REQUESTED = 2;

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

bool rd::GameState::onKeyDown(rd::RdKey k)
{
    if (k.getValue()==rd::RdKey::KEY_SPACE)
    {
        mentalMap->shoot();
        return true;
    }

    if (k.getChar()=='r')
    {
        mentalMap->reload();
        return true;
    }

    if (k.getValue() == RdKey::KEY_ESCAPE)
    {
        RD_DEBUG("Escape was pressed!\n");
        received_exit = true;
        return true;
    }

    //-- Movement control
    if (k.getValue() == RdKey::KEY_ARROW_LEFT)
    {
        RD_DEBUG("Left arrow was pressed!\n");
        robotManager->turnLeft();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_RIGHT)
    {
        RD_DEBUG("Right arrow was pressed!\n");
        robotManager->turnRight();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_UP)
    {
        RD_DEBUG("Up arrow was pressed!\n");
        robotManager->moveForward();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_DOWN)
    {
        RD_DEBUG("Down arrow was pressed!\n");
        robotManager->moveBackwards();
        return true;
    }

    return false;
}

bool rd::GameState::onKeyUp(rd::RdKey k)
{
    //-- Movement control
    if (k.getValue() == RdKey::KEY_ARROW_LEFT)
    {
        RD_DEBUG("Left arrow was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_RIGHT)
    {
        RD_DEBUG("Right arrow was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_UP)
    {
        RD_DEBUG("Up arrow was released!\n");
        robotManager->stopMovement();
        return true;
    }
    if (k.getValue() == RdKey::KEY_ARROW_DOWN)
    {
        RD_DEBUG("Down arrow was released!\n");
        robotManager->stopMovement();
        return true;
    }

    return false;
}

bool rd::GameState::onImageArrived(rd::RdImageManager *manager)
{
    //-- Don't know if this is safe enough or some mutex is required
    screenManager->update(GameScreen::PARAM_CAMERA_FRAME, manager->getImage());
}

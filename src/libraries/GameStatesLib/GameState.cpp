#include "GameState.hpp"

const int rd::GameState::KILLED = 1;
const int rd::GameState::EXIT_REQUESTED = 2;

rd::GameState::GameState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, rd::AudioManager *audioManager) :
                    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
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
        imageManager->start();
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

    //-- Robot Startup goes here

    //-- Show Robot Devastation game screen:
    //-- Set info elements on GameScreen
    screen.update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screen.update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screen.update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screen.update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());
    screen.show();

    return true;
}

bool rd::GameState::loop()
{
    //-- Set info elements on GameScreen
    screen.update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screen.update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screen.update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screen.update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());
    screen.show();
    return true;
}

bool rd::GameState::cleanup()
{
    if (received_exit)
    {
        //-- Stop things to exit game (logout)
        imageManager->stop();
        inputManager->stop();
        inputManager->removeInputEventListeners();
        audioManager->stopMusic();
        audioManager->stop();
        networkManager->logout(mentalMap->getMyself()); //-- This is kind of weird, but it is supposed to be done like this
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

    return false;
}

bool rd::GameState::onKeyUp(rd::RdKey k)
{
    return false;
}

bool rd::GameState::onImageArrived(rd::RdImageManager *manager)
{
    //-- Don't know if this is safe enough or some mutex is required
    screen.update(GameScreen::PARAM_CAMERA_FRAME, manager->getImage());
}

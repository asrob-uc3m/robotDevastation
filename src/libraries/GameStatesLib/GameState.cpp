#include "GameState.hpp"

const int rd::GameState::KILLED = 1;
const int rd::GameState::QUIT_REQUESTED = 2;

rd::GameState::GameState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, rd::AudioManager *audioManager) :
                    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
{
    this->state_id = "GameState";
}

rd::GameState::~GameState()
{

}

bool rd::GameState::setup()
{
    //-- Connect mentalMap to networkManager to receive updates
    if (mentalMap!=NULL && networkManager!=NULL)
    {
        networkManager->addNetworkEventListener(mentalMap);
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

    return true;
}

bool rd::GameState::loop()
{
    return true;
}

bool rd::GameState::cleanup()
{
    return false;
}

int rd::GameState::evaluateConditions()
{
    if (mentalMap->getMyself().getHealth()==0)
    {
        return KILLED;
    }

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

    return false;
}

bool rd::GameState::onKeyUp(rd::RdKey k)
{
    return false;
}

#include "DeadState.hpp"

const int rd::DeadState::RESPAWN_SELECTED = 1;
const int rd::DeadState::EXIT_SELECTED = 2;


rd::DeadState::DeadState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager) :
                ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
{

}

rd::DeadState::~DeadState()
{

}

bool rd::DeadState::setup()
{
    return false;
}

bool rd::DeadState::loop()
{
    return false;
}

bool rd::DeadState::cleanup()
{
    return false;
}

int rd::DeadState::evaluateConditions()
{
    return false;
}

bool rd::DeadState::onKeyDown(rd::RdKey k)
{
    return false;
}

bool rd::DeadState::onKeyUp(rd::RdKey k)
{
    return false;
}

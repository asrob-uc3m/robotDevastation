#include "GameState.hpp"

const int rd::GameState::KILLED = 1;
const int rd::GameState::QUIT_REQUESTED = 2;

rd::GameState::GameState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager, rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap, rd::RdRobotManager *robotManager, rd::AudioManager *audioManager)
{

}

rd::GameState::~GameState()
{

}

bool rd::GameState::setup()
{
    return false;
}

bool rd::GameState::loop()
{
    return false;
}

bool rd::GameState::cleanup()
{
    return false;
}

int rd::GameState::evaluateConditions()
{
    return -1;
}

bool rd::GameState::onKeyDown(rd::RdKey k)
{
    return false;
}

bool rd::GameState::onKeyUp(rd::RdKey k)
{
    return false;
}

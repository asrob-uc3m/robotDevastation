#include "InitState.hpp"

rd::InitState::InitState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager) :
    ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
{

}

rd::InitState::~InitState()
{

}

bool rd::InitState::setup()
{
    return false;
}

bool rd::InitState::loop()
{
    return false;
}

bool rd::InitState::cleanup()
{
    return false;
}

int rd::InitState::evaluateConditions()
{
    return false;
}

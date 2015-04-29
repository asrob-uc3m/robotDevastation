// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdMockupRobotManager.hpp"

namespace rd
{

bool RdMockupRobotManager::moveForward(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::moveBackwards(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::turnLeft(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::turnRight(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::stopMovement() {
    RD_DEBUG("\n");
    return true;
}

bool RdMockupRobotManager::tiltUp(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::tiltDown(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::panLeft(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}

bool RdMockupRobotManager::panRight(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return true;
}
        
bool RdMockupRobotManager::connect()  {
    RD_DEBUG("\n");
    return true;
}

bool RdMockupRobotManager::disconnect()  {
    RD_DEBUG("\n");
    return true;
}

bool RdMockupRobotManager::test()  {
    RD_DEBUG("\n");
    return true;
}

bool RdMockupRobotManager::ping()  {
    RD_DEBUG("\n");
    return true;
}

void RdMockupRobotManager::onDestroy(){
    RD_DEBUG("\n");
    return;
}

} //rd

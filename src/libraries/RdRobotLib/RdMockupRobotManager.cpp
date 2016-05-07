// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdMockupRobotManager.hpp"

const int rd::RdMockupRobotManager::FORWARD = 1;
const int rd::RdMockupRobotManager::BACKWARDS = 2;
const int rd::RdMockupRobotManager::LEFT = 4;
const int rd::RdMockupRobotManager::RIGHT = 8;
const int rd::RdMockupRobotManager::NONE = 0;

const int rd::RdMockupRobotManager::CAMERA_UP = 1;
const int rd::RdMockupRobotManager::CAMERA_DOWN = 2;
const int rd::RdMockupRobotManager::CAMERA_LEFT = 4;
const int rd::RdMockupRobotManager::CAMERA_RIGHT = 8;
const int rd::RdMockupRobotManager::CAMERA_NONE = 0; 

namespace rd
{

bool RdMockupRobotManager::moveForward(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::moveBackwards(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::turnLeft(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::turnRight(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::stopMovement() {
    RD_DEBUG("\n");
    return false;
}

bool RdMockupRobotManager::tiltUp(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::tiltDown(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::panLeft(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::panRight(int velocity) {
    RD_DEBUG("(%d)\n",velocity)
    return false;
}

bool RdMockupRobotManager::stopCameraMovement() {
    return false;
}
        
bool RdMockupRobotManager::connect()  {
    RD_DEBUG("\n");
    return false;
}

bool RdMockupRobotManager::disconnect()  {
    RD_DEBUG("\n");
    return false;
}

bool RdMockupRobotManager::test()  {
    RD_DEBUG("\n");
    return false;
}

bool RdMockupRobotManager::ping()  {
    RD_DEBUG("\n");
    return false;
}

void RdMockupRobotManager::onDestroy(){
    RD_DEBUG("\n");
    return;
}

bool RdMockupRobotManager::isConnected() {
    return false;
}

bool RdMockupRobotManager::isMoving() {
    return false;
}

int RdMockupRobotManager::getMovementDirection() {
    return -1;
}

bool RdMockupRobotManager::isCameraMoving() {
    return false;
}

int RdMockupRobotManager::getCameraMovementDirection() {
    return -1;
}

} //rd

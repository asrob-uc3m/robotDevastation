// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockRobotManager.hpp"

#include <ColorDebug.hpp>

const int rd::MockRobotManager::FORWARD = 1;
const int rd::MockRobotManager::BACKWARDS = 2;
const int rd::MockRobotManager::LEFT = 4;
const int rd::MockRobotManager::RIGHT = 8;
const int rd::MockRobotManager::NONE = 0;

const int rd::MockRobotManager::CAMERA_UP = 1;
const int rd::MockRobotManager::CAMERA_DOWN = 2;
const int rd::MockRobotManager::CAMERA_LEFT = 4;
const int rd::MockRobotManager::CAMERA_RIGHT = 8;
const int rd::MockRobotManager::CAMERA_NONE = 0; 

rd::MockRobotManager::MockRobotManager(const std::string& robotName)
{
    this->robotName = robotName;
    movement_direction = NONE;
    camera_movement_direction = CAMERA_NONE;
}

bool rd::MockRobotManager::moveForward(double value)
{
    movement_direction = FORWARD;
    return true;
}

bool rd::MockRobotManager::turnLeft(double value)
{
    movement_direction = LEFT;
    return true;
}

bool rd::MockRobotManager::stopMovement()
{
    movement_direction = NONE;
    return true;
}

bool rd::MockRobotManager::tiltDown(double value)
{
    camera_movement_direction = CAMERA_DOWN;
    return true;
}

bool rd::MockRobotManager::panLeft(double value)
{
    camera_movement_direction = CAMERA_LEFT;
    return true;
}

bool rd::MockRobotManager::stopCameraMovement()
{
    camera_movement_direction = NONE;
    return true;
}
        
bool rd::MockRobotManager::isMoving() const
{
    return movement_direction!=NONE;
}

int rd::MockRobotManager::getMovementDirection()
{
    return movement_direction;
}

bool rd::MockRobotManager::isCameraMoving()
{
    return camera_movement_direction != NONE;
}

int rd::MockRobotManager::getCameraMovementDirection()
{
    return camera_movement_direction;
}

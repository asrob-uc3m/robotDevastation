// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockRobotManager.hpp"

#include <ColorDebug.hpp>

const int rd::MockRobotManager::NONE = 0;
const int rd::MockRobotManager::FORWARD = 1;
const int rd::MockRobotManager::BACKWARDS = 2;
const int rd::MockRobotManager::LEFT = 3;
const int rd::MockRobotManager::RIGHT = 4;

const int rd::MockRobotManager::CAMERA_NONE = 5;
const int rd::MockRobotManager::CAMERA_DOWN = 6;
const int rd::MockRobotManager::CAMERA_UP = 7;
const int rd::MockRobotManager::CAMERA_LEFT = 8;
const int rd::MockRobotManager::CAMERA_RIGHT = 9;

rd::MockRobotManager::MockRobotManager(const std::string& robotName)
{
    this->robotName = robotName;
    movement_direction = NONE;
    camera_movement_direction = CAMERA_NONE;
}

bool rd::MockRobotManager::moveForward(double value)
{
    if(value > 0)
        movement_direction = FORWARD;
    else if(value < 0)
        movement_direction = BACKWARDS;
    else
        movement_direction = NONE;
    return true;
}

bool rd::MockRobotManager::turnLeft(double value)
{
    if(value > 0)
        movement_direction = LEFT;
    else if(value < 0)
        movement_direction = RIGHT;
    else
        movement_direction = NONE;
    return true;
}

bool rd::MockRobotManager::stopMovement()
{
    movement_direction = NONE;
    return true;
}

bool rd::MockRobotManager::tiltDown(double value)
{
    if(value > 0)
        camera_movement_direction = CAMERA_DOWN;
    else if(value < 0)
        camera_movement_direction = CAMERA_UP;
    else
        camera_movement_direction = CAMERA_NONE;
    return true;
}

bool rd::MockRobotManager::panLeft(double value)
{
    if(value > 0)
        camera_movement_direction = CAMERA_LEFT;
    else if(value < 0)
        camera_movement_direction = CAMERA_RIGHT;
    else
        camera_movement_direction = CAMERA_NONE;
    return true;
}

bool rd::MockRobotManager::stopCameraMovement()
{
    camera_movement_direction = CAMERA_NONE;
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
    return camera_movement_direction != CAMERA_NONE;
}

int rd::MockRobotManager::getCameraMovementDirection()
{
    return camera_movement_direction;
}

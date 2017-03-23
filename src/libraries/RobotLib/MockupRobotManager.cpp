// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "MockupRobotManager.hpp"

const int rd::MockupRobotManager::FORWARD = 1;
const int rd::MockupRobotManager::BACKWARDS = 2;
const int rd::MockupRobotManager::LEFT = 4;
const int rd::MockupRobotManager::RIGHT = 8;
const int rd::MockupRobotManager::NONE = 0;

const int rd::MockupRobotManager::CAMERA_UP = 1;
const int rd::MockupRobotManager::CAMERA_DOWN = 2;
const int rd::MockupRobotManager::CAMERA_LEFT = 4;
const int rd::MockupRobotManager::CAMERA_RIGHT = 8;
const int rd::MockupRobotManager::CAMERA_NONE = 0; 

namespace rd
{
MockupRobotManager:: MockupRobotManager(const std::string& robotName): RobotManager(robotName)
{ 
    connected = false;
    enabled = false;
    movement_direction = NONE;
    camera_movement_direction = CAMERA_NONE;
}

bool MockupRobotManager::moveForward(int velocity) 
{
    if (connected)
    {
        if (enabled)
        {
            movement_direction = FORWARD;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::moveBackwards(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            movement_direction = BACKWARDS;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::turnLeft(int velocity) 
{
    if (connected)
    {
        if (enabled)
        {
            movement_direction = LEFT;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::turnRight(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            movement_direction = RIGHT;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::stopMovement()
{
    if (connected)
    {
        if (enabled)
        {
            movement_direction = NONE;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::tiltUp(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            camera_movement_direction = CAMERA_UP;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::tiltDown(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            camera_movement_direction = CAMERA_DOWN;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::panLeft(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            camera_movement_direction = CAMERA_LEFT;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::panRight(int velocity)
{
    if (connected)
    {
        if (enabled)
        {
            camera_movement_direction = CAMERA_RIGHT;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::stopCameraMovement()
{
    if (connected)
    {
        if (enabled)
        {
            camera_movement_direction = NONE;
            return true;
        }
        else
        {
            RD_ERROR("Robot is disabled\n");
            return false;
        }
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}
        
bool MockupRobotManager::connect()  
{
    if (!connected)
    {
	connected = true;
        return true;
    }
    else
    {
        RD_WARNING("Already connected\n");
        return true;
    }
}

bool MockupRobotManager::disconnect()  
{
    if (connected)
    {
	connected = false;
        return true;
    }
    else
    {
        RD_ERROR("Not connected\n");
        return false;
    }
}

bool MockupRobotManager::test()  {
    RD_DEBUG("\n");
    return false;
}

void MockupRobotManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    if (enabled)
    {
        RD_DEBUG("RdMockupRobotManager enabled\n");
    }
    else
        RD_DEBUG("RdMockupRobotManager disabled\n");
}

void MockupRobotManager::onDestroy(){
    RD_DEBUG("\n");
    return;
}

bool MockupRobotManager::isConnected()
{
    return connected;
}

bool MockupRobotManager::isEnabled()
{
    return enabled;
}

bool MockupRobotManager::isMoving() 
{
    return movement_direction!=NONE;
}

int MockupRobotManager::getMovementDirection() 
{
    return movement_direction;
}

bool MockupRobotManager::isCameraMoving()
{
    return camera_movement_direction != NONE;
}

int MockupRobotManager::getCameraMovementDirection()
{
    return camera_movement_direction;
}

} //rd

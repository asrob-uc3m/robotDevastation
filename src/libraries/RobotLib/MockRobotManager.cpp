// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "MockRobotManager.hpp"
#include "Macros.hpp"

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

namespace rd
{
MockRobotManager::MockRobotManager(const std::string& robotName)
{
    this->robotName = robotName;
    connected = false;
    enabled = false;
    movement_direction = NONE;
    camera_movement_direction = CAMERA_NONE;
}

bool MockRobotManager::moveForward(int velocity) 
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

bool MockRobotManager::moveBackwards(int velocity)
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

bool MockRobotManager::turnLeft(int velocity) 
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

bool MockRobotManager::turnRight(int velocity)
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

bool MockRobotManager::stopMovement()
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

bool MockRobotManager::tiltUp(int velocity)
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

bool MockRobotManager::tiltDown(int velocity)
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

bool MockRobotManager::panLeft(int velocity)
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

bool MockRobotManager::panRight(int velocity)
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

bool MockRobotManager::stopCameraMovement()
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
        
bool MockRobotManager::connect()  
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

bool MockRobotManager::disconnect()  
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

bool MockRobotManager::test()  {
    RD_DEBUG("\n");
    return false;
}

void MockRobotManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    if (enabled)
    {
        RD_DEBUG("MockRobotManager enabled\n");
    }
    else
        RD_DEBUG("MockRobotManager disabled\n");
}

void MockRobotManager::onDestroy(){
    RD_DEBUG("\n");
    return;
}

bool MockRobotManager::isConnected() const
{
    return connected;
}

bool MockRobotManager::isEnabled() const
{
    return enabled;
}

bool MockRobotManager::isMoving() const
{
    return movement_direction!=NONE;
}

int MockRobotManager::getMovementDirection() 
{
    return movement_direction;
}

bool MockRobotManager::isCameraMoving()
{
    return camera_movement_direction != NONE;
}

int MockRobotManager::getCameraMovementDirection()
{
    return camera_movement_direction;
}

} //rd

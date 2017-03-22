// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "MockupRobotManager.hpp"

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
RdMockupRobotManager:: RdMockupRobotManager(const std::string& robotName): RdRobotManager(robotName)
{ 
    connected = false;
    enabled = false;
    movement_direction = NONE;
    camera_movement_direction = CAMERA_NONE;
}

bool RdMockupRobotManager::moveForward(int velocity) 
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

bool RdMockupRobotManager::moveBackwards(int velocity)
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

bool RdMockupRobotManager::turnLeft(int velocity) 
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

bool RdMockupRobotManager::turnRight(int velocity)
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

bool RdMockupRobotManager::stopMovement()
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

bool RdMockupRobotManager::tiltUp(int velocity)
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

bool RdMockupRobotManager::tiltDown(int velocity)
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

bool RdMockupRobotManager::panLeft(int velocity)
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

bool RdMockupRobotManager::panRight(int velocity)
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

bool RdMockupRobotManager::stopCameraMovement()
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
        
bool RdMockupRobotManager::connect()  
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

bool RdMockupRobotManager::disconnect()  
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

bool RdMockupRobotManager::test()  {
    RD_DEBUG("\n");
    return false;
}

void RdMockupRobotManager::setEnabled(bool enabled)
{
    this->enabled = enabled;
    if (enabled)
    {
        RD_DEBUG("RdMockupRobotManager enabled\n");
    }
    else
        RD_DEBUG("RdMockupRobotManager disabled\n");
}

void RdMockupRobotManager::onDestroy(){
    RD_DEBUG("\n");
    return;
}

bool RdMockupRobotManager::isConnected()
{
    return connected;
}

bool RdMockupRobotManager::isEnabled()
{
    return enabled;
}

bool RdMockupRobotManager::isMoving() 
{
    return movement_direction!=NONE;
}

int RdMockupRobotManager::getMovementDirection() 
{
    return movement_direction;
}

bool RdMockupRobotManager::isCameraMoving()
{
    return camera_movement_direction != NONE;
}

int RdMockupRobotManager::getCameraMovementDirection()
{
    return camera_movement_direction;
}

} //rd

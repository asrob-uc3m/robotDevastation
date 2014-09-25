#include "RdTextualRobotManager.hpp"


bool rd::RdTextualRobotManager::moveForward(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Robot is moving forward with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::moveBackwards(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Robot is moving backwards with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::turnLeft(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Robot is turning left with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::turnRight(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Robot is turning right with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::tiltUp(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Camera is tilting up with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::tiltDown(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Camera is tilting down with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::panLeft(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Camera is panning left with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::panRight(int time, int velocity)
{
    std::stringstream sstream_time, sstream_velocity;
    if (time == UNUSED)
        sstream_time << "not specified";
    else
        sstream_time << time;

    if (velocity == UNUSED)
        sstream_velocity << "not specified";
    else
        sstream_velocity << velocity;

    RD_SUCCESS("Camera is panning right with v=%s during t=%s\n",
               sstream_velocity.str().c_str(), sstream_time.str().c_str());

    return true;
}

bool rd::RdTextualRobotManager::connect()
{
    if (connection_status == DISCONNECTED)
    {
        connection_status = CONNECTED;
        RD_SUCCESS("Connection with robot stablished!\n");
        return true;
    }
    else
    {
        RD_ERROR("Robot already connected!\n");
        return false;
    }

}

bool rd::RdTextualRobotManager::disconnect()
{
    if (connection_status == CONNECTED)
    {
        connection_status = DISCONNECTED;
        RD_SUCCESS("Disconnected from robot!\n");
        return true;
    }
    else
    {
        RD_ERROR("Robot is not currenly connected!\n");
        return false;
    }

}

bool rd::RdTextualRobotManager::test()
{
    RD_SUCCESS("Test function!\n");
    return true;
}

bool rd::RdTextualRobotManager::ping()
{
    if (connection_status == CONNECTED)
    {
        RD_SUCCESS("I'm still here!\n");
        return true;
    }
    else
    {
        RD_ERROR("Lost connection!\n");
        return false;
    }
}

void rd::RdTextualRobotManager::onDestroy()
{
    disconnect();
}

rd::RdTextualRobotManager::RdTextualRobotManager()
{
    connection_status = DISCONNECTED;
    RdRobotManager::registerManager("textualRobot", this);
}

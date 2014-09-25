// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdRd1RobotManager.hpp"

namespace rd{

bool RdRd1RobotManager::moveForward(int velocity) {
    double velocities[] = {1000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdRd1RobotManager::moveBackwards(int velocity) {
    double velocities[] = {2000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdRd1RobotManager::turnLeft(int velocity) {
    double velocities[] = {1000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdRd1RobotManager::turnRight(int velocity) {
    double velocities[] = {2000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdRd1RobotManager::stopMovement() {
    vel->stop();
    return true;
}

bool RdRd1RobotManager::tiltUp(int velocity) {
    return true;
}

bool RdRd1RobotManager::tiltDown(int velocity) {
    return true;
}

bool RdRd1RobotManager::panLeft(int velocity) {
    return true;
}

bool RdRd1RobotManager::panRight(int velocity) {
    return true;
}
        
bool RdRd1RobotManager::connect()  {

    yarp::os::Property robotOptions;
    robotOptions.put("device","controlboard");
    robotOptions.put("subdevice","remote_controlboard");
    robotOptions.put("local","/pleaseChangeMyName");
    robotOptions.put("remote","/raspi");
    robotDevice.open(robotOptions);

    bool ok = true;
    ok &= robotDevice.view(vel);

    return ok;
}

bool RdRd1RobotManager::disconnect()  {
    robotDevice.close();
    return true;
}

bool RdRd1RobotManager::test()  {
    return true;
}

bool RdRd1RobotManager::ping()  {
    return true;
}

}
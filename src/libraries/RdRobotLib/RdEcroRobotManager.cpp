// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdEcroRobotManager.hpp"

namespace rd{

bool RdEcroRobotManager::moveForward(int velocity) {
    double velocities[] = {2000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdEcroRobotManager::moveBackwards(int velocity) {
    double velocities[] = {1000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdEcroRobotManager::turnLeft(int velocity) {
    double velocities[] = {1000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdEcroRobotManager::turnRight(int velocity) {
    double velocities[] = {2000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdEcroRobotManager::stopMovement() {
    //vel->stop();
    double velocities[] = {0,0};
    vel->velocityMove(velocities);
    return true;
}

bool RdEcroRobotManager::tiltUp(int velocity) {
    return true;
}

bool RdEcroRobotManager::tiltDown(int velocity) {
    return true;
}

bool RdEcroRobotManager::panLeft(int velocity) {
    return true;
}

bool RdEcroRobotManager::panRight(int velocity) {
    return true;
}
        
bool RdEcroRobotManager::connect()  {

    std::ostringstream local_s;
    local_s << "/";
    local_s << playerId;
    local_s << "/robot";

    std::ostringstream remote_s;
    remote_s << "/";
    remote_s << playerId;
    remote_s << "/ecro";

    yarp::os::Property robotOptions;
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("local", local_s.str().c_str() );
    robotOptions.put("remote", remote_s.str().c_str() );
    robotDevice.open(robotOptions);

    bool ok = true;
    ok &= robotDevice.view(vel);
    if(!ok)
    {
        RD_ERROR("Could not connect to robot motors.\n");
        return false;
    }
    RD_SUCCESS("Connected to robot motors.\n");

    return true;
}

bool RdEcroRobotManager::disconnect()  {
    robotDevice.close();
    return true;
}

bool RdEcroRobotManager::test()  {
    return true;
}

bool RdEcroRobotManager::ping()  {
    return true;
}

void RdEcroRobotManager::onDestroy(){
    return;
}

} //rd

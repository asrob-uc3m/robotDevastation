// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdYarpRobotManager.hpp"

namespace rd{

bool RdYarpRobotManager::moveForward(int velocity) {
    double velocities[] = {2000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::moveBackwards(int velocity) {
    double velocities[] = {1000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::turnLeft(int velocity) {
    double velocities[] = {1000,1000};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::turnRight(int velocity) {
    double velocities[] = {2000,2000};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::stopMovement() {
    vel->stop();
    return true;
}

bool RdYarpRobotManager::tiltUp(int velocity) {
    return true;
}

bool RdYarpRobotManager::tiltDown(int velocity) {
    return true;
}

bool RdYarpRobotManager::panLeft(int velocity) {
    return true;
}

bool RdYarpRobotManager::panRight(int velocity) {
    return true;
}
        
bool RdYarpRobotManager::connect()  {

    std::ostringstream local_s;
    local_s << "/";
    local_s << playerId;
    local_s << "/robot";

    std::ostringstream remote_s;
    remote_s << "/";
    remote_s << playerId;
    remote_s << "/rd1";

    yarp::os::Property robotOptions;
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("local", local_s.str().c_str() );
    robotOptions.put("remote", remote_s.str().c_str() );
    robotDevice.open(robotOptions);

    if( ! robotDevice.isValid() ) {
        RD_ERROR("Could not connect to remote robot.\n");
        return false;
    }
    RD_SUCCESS("Connected to remote robot.\n");

    if(! robotDevice.view(vel) )
    {
        RD_ERROR("Could not aquire robot motor velocity interface.\n");
        return false;
    }
    RD_SUCCESS("Acquired robot motor velocity interface.\n");

    return true;
}

bool RdYarpRobotManager::disconnect()  {
    robotDevice.close();
    return true;
}

bool RdYarpRobotManager::test()  {
    return true;
}

bool RdYarpRobotManager::ping()  {
    return true;
}

void RdYarpRobotManager::onDestroy(){
    return;
}

} //rd

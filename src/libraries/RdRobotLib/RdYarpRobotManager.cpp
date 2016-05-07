// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdYarpRobotManager.hpp"

namespace rd{

bool RdYarpRobotManager::moveForward(int velocity) {
    double velocities[] = {100,100};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::moveBackwards(int velocity) {
    double velocities[] = {-100,-100};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::turnLeft(int velocity) {
    double velocities[] = {-100,100};
    vel->velocityMove(velocities);
    return true;
}

bool RdYarpRobotManager::turnRight(int velocity) {
    double velocities[] = {100,-100};
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

bool RdYarpRobotManager::stopCameraMovement() {
    return true;
}
        
bool RdYarpRobotManager::connect()  {

    std::string launchRobotOptionsStr("(on /");
    launchRobotOptionsStr += robotName;
    launchRobotOptionsStr += ") (as roblauncher) (cmd \"sudo launchRaspiYarp --device OnePwmMotors --name /";
    launchRobotOptionsStr += robotName;
    launchRobotOptionsStr += " --gpios 17 27\")";
    yarp::os::Property launchRobotOptions;
    launchRobotOptions.fromString(launchRobotOptionsStr);
    RD_INFO("Attempting to start robot launch on robot side...\n");
    RD_INFO("If you prefer a fake robot with a fake camera, launch 'robotDevastation --mockupRobotManager --mockupImageManager'\n");
    int robotRet = yarp::os::Run::client(launchRobotOptions);
    if (robotRet != 0)
    {
        RD_ERROR("Could not start robot launch on robot side.\n");
        return false;
    }
    RD_SUCCESS("Started robot launch on robot side.\n");

    std::string launchCameraOptionsStr("(on /");
    launchCameraOptionsStr += robotName;
    launchCameraOptionsStr += ") (as launcher) (cmd \"yarpdev --device opencv_grabber --name /";
    launchCameraOptionsStr += robotName;
    launchCameraOptionsStr += "/img:o\")";
    yarp::os::Property launchCameraOptions;
    launchCameraOptions.fromString(launchCameraOptionsStr);
    RD_INFO("Attempting to start camera launch on robot side...\n");
    RD_INFO("If you prefer a fake robot with a fake camera, launch 'robotDevastation --mockupRobotManager --mockupImageManager'\n");
    int cameraRet = yarp::os::Run::client(launchCameraOptions);
    if (cameraRet != 0)
    {
        RD_ERROR("Could not start camera launch on robot side.\n");
        return false;
    }
    RD_SUCCESS("Started camera launch on robot side.\n");

    std::string local_s("/robotDevastation/");
    local_s += robotName;

    std::string remote_s("/");
    remote_s += robotName;

    yarp::os::Property robotOptions;
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("local", local_s );
    robotOptions.put("remote", remote_s );

    int tries = 0;
    while(tries++ < 10)
    {
        if( !! robotDevice.isValid() )
            break;
        RD_DEBUG("Wait to connect to remote robot, try %d...\n",tries);
        yarp::os::Time::delay(0.5);
        robotDevice.open(robotOptions);
    }

    if (tries == 11)
    {
        RD_ERROR("Timeout on connect to remote robot!\n");
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

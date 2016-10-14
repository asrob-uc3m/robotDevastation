// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdYarpRobotManager.hpp"

namespace rd
{
RdYarpRobotManager::RdYarpRobotManager(const std::string& robotName): RdRobotManager(robotName)
{
    connected = false;
    enabled = false;
}

bool RdYarpRobotManager::moveForward(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_FORWARD,velocity);
}

bool RdYarpRobotManager::moveBackwards(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_BACKWARDS,velocity);
}

bool RdYarpRobotManager::turnLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_LEFT,velocity);
}

bool RdYarpRobotManager::turnRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_RIGHT,velocity);
}

bool RdYarpRobotManager::stopMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_MOVEMENT);
}

bool RdYarpRobotManager::tiltUp(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_UP,velocity);
}

bool RdYarpRobotManager::tiltDown(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_DOWN,velocity);
}

bool RdYarpRobotManager::panLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_LEFT,velocity);
}

bool RdYarpRobotManager::panRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_RIGHT,velocity);
}

bool RdYarpRobotManager::stopCameraMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_CAMERA_MOVEMENT);
}
        
bool RdYarpRobotManager::connect()
{
    if( connected )
    {
        RD_WARNING("Already connected\n");
        return true;
    }

    yarp::os::NetworkBase::initMinimum();

    if ( ! yarp::os::NetworkBase::checkNetwork() )
    {
        RD_ERROR("Found no yarp network (try running 'yarpserver &'', or '--mockupRobotManager' for Fake robot motors). Bye!\n");
        return false;
    }

    std::string launchRobotOptionsStr("(on /");
    launchRobotOptionsStr += robotName;
    launchRobotOptionsStr += ") (as roblauncher) (cmd \"sudo launchRaspiYarp --device RdRobotServer --subdevice RdOnePwmMotors --name /";  // RdOnePwmMotors or RdFakeMotors
    launchRobotOptionsStr += robotName;
    launchRobotOptionsStr += " --gpios 17 27\")";
    yarp::os::Property launchRobotOptions;
    launchRobotOptions.fromString(launchRobotOptionsStr);  //-- Default should look like "/rd1/rpc:s"
    RD_DEBUG("Attempting to start motors on robot side [parameters: %s]...\n",launchRobotOptionsStr.c_str());
    RD_INFO("If you prefer a fake robot with a fake camera, launch 'robotDevastation --mockupRobotManager --mockupImageManager'\n");
    int robotRet = yarp::os::Run::client(launchRobotOptions);
    if (robotRet == 0)
    {
        RD_SUCCESS("Started motors on robot side.\n");
    }
    else
    {
        RD_WARNING("Could not start motors on robot side, but will atempt to connect anyway.\n");
    }

    std::string launchCameraOptionsStr("(on /");
    launchCameraOptionsStr += robotName;
    launchCameraOptionsStr += ") (as launcher) (cmd \"yarpdev --device opencv_grabber --name /";
    launchCameraOptionsStr += robotName;
    launchCameraOptionsStr += "/img:o\")";
    yarp::os::Property launchCameraOptions;
    launchCameraOptions.fromString(launchCameraOptionsStr);
    RD_DEBUG("Attempting to start camera on robot side [parameters: %s]...\n",launchCameraOptionsStr.c_str());
    RD_INFO("If you prefer a fake robot with a fake camera, launch 'robotDevastation --mockupRobotManager --mockupImageManager'\n");
    int cameraRet = yarp::os::Run::client(launchCameraOptions);  //-- Default should look like "/rd1/img:o"
    if (cameraRet == 0)
    {
        RD_SUCCESS("Started camera on robot side.\n");
    }
    else
    {
        RD_WARNING("Could not start camera on robot side, but will atempt to connect anyway.\n");
    }

    std::string local_s("/robotDevastation/");
    local_s += robotName;
    local_s += "/rpc:c";

    rpcClient.open(local_s);  //-- Default should look like "/robotDevastation/rd1/rpc:c"

    std::string remote_s("/");
    remote_s += robotName;
    remote_s += "/rpc:s";

    int tries = 0;
    while(tries++ < 10)
    {
        yarp::os::Network::connect(local_s,remote_s);
        if( rpcClient.getOutputCount() > 0) break;
        RD_DEBUG("Wait to connect to remote robot, try %d...\n",tries);
        yarp::os::Time::delay(0.5);
    }

    if (tries == 11)
    {
        RD_ERROR("Timeout on connect to remote robot! If you prefer a fake robot with a fake camera, launch 'robotDevastation --mockupRobotManager --mockupImageManager'\n");
        return false;
    }

    RD_SUCCESS("Connected to remote robot.\n");

    return true;
}

bool RdYarpRobotManager::disconnect()  {
    rpcClient.close();
    return true;
}

bool RdYarpRobotManager::test()  {
    return true;
}

void RdYarpRobotManager::setEnabled(bool enabled)
{

}

void RdYarpRobotManager::onDestroy(){
    return;
}

bool RdYarpRobotManager::send1vocab1int(int vocab, int integer)
{
    yarp::os::Bottle cmd, response;
    cmd.addVocab(vocab);
    cmd.addInt(integer);
    rpcClient.write(cmd,response);
    if( response.get(0).asVocab() == VOCAB_OK )
        return true;
    else
        return false;
}

bool RdYarpRobotManager::send1vocab(int vocab)
{
    yarp::os::Bottle cmd, response;
    cmd.addVocab(vocab);
    rpcClient.write(cmd,response);
    if( response.get(0).asVocab() == VOCAB_OK )
        return true;
    else
        return false;
}

} //rd

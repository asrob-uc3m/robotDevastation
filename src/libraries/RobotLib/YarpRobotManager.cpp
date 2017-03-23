// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "YarpRobotManager.hpp"

namespace rd
{
YarpRobotManager::YarpRobotManager(const std::string& robotName): RobotManager(robotName)
{
    connected = false;
    enabled = false;
}

bool YarpRobotManager::moveForward(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_FORWARD,velocity);
}

bool YarpRobotManager::moveBackwards(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_BACKWARDS,velocity);
}

bool YarpRobotManager::turnLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_LEFT,velocity);
}

bool YarpRobotManager::turnRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_RIGHT,velocity);
}

bool YarpRobotManager::stopMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_MOVEMENT);
}

bool YarpRobotManager::tiltUp(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_UP,velocity);
}

bool YarpRobotManager::tiltDown(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_DOWN,velocity);
}

bool YarpRobotManager::panLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_LEFT,velocity);
}

bool YarpRobotManager::panRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_RIGHT,velocity);
}

bool YarpRobotManager::stopCameraMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_CAMERA_MOVEMENT);
}
        
bool YarpRobotManager::connect()
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

    std::string yarprunPortStr("/");
    yarprunPortStr += robotName;
    if( yarp::os::NetworkBase::exists(yarprunPortStr) )
    {
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
    }
    else
    {
        RD_WARNING("No %s yarprun port found. Will try to connect, but jump remote launching.\n", yarprunPortStr.c_str());
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
        RD_ERROR("Timeout on connect to remote robot!\n");
        RD_INFO("If you prefer a fake robot use the '--mockupRobotManager' parameter to run robotDevastation.\n");
        return false;
    }

    RD_SUCCESS("Connected to remote robot.\n");

    return true;
}

bool YarpRobotManager::disconnect()  {
    rpcClient.close();
    return true;
}

bool YarpRobotManager::test()  {
    return true;
}

void YarpRobotManager::setEnabled(bool enabled)
{

}

void YarpRobotManager::onDestroy(){
    return;
}

bool YarpRobotManager::send1vocab1int(int vocab, int integer)
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

bool YarpRobotManager::send1vocab(int vocab)
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

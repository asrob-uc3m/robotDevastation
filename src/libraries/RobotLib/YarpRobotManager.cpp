// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "YarpRobotManager.hpp"

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

#include "Macros.hpp"

rd::YarpRobotManager::YarpRobotManager(const std::string& robotName)
{
    this->robotName = robotName;
    connected = false;
    enabled = false;
}

bool rd::YarpRobotManager::moveForward(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_FORWARD,velocity);
}

bool rd::YarpRobotManager::moveBackwards(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_MOVE_BACKWARDS,velocity);
}

bool rd::YarpRobotManager::turnLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_LEFT,velocity);
}

bool rd::YarpRobotManager::turnRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TURN_RIGHT,velocity);
}

bool rd::YarpRobotManager::stopMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_MOVEMENT);
}

bool rd::YarpRobotManager::tiltUp(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_UP,velocity);
}

bool rd::YarpRobotManager::tiltDown(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_TILT_DOWN,velocity);
}

bool rd::YarpRobotManager::panLeft(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_LEFT,velocity);
}

bool rd::YarpRobotManager::panRight(int velocity)
{
    RD_DEBUG("\n");
    return send1vocab1int(VOCAB_PAN_RIGHT,velocity);
}

bool rd::YarpRobotManager::stopCameraMovement()
{
    RD_DEBUG("\n");
    return send1vocab(VOCAB_STOP_CAMERA_MOVEMENT);
}
        
bool rd::YarpRobotManager::connect()
{
    if( connected )
    {
        RD_WARNING("Already connected\n");
        return true;
    }

    yarp::os::NetworkBase::initMinimum();

    if ( ! yarp::os::NetworkBase::checkNetwork() )
    {
        RD_ERROR("Found no yarp network (try running 'yarpserver &'', or '--mockRobotManager' for Fake robot motors). Bye!\n");
        return false;
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
        RD_INFO("If you prefer a fake robot use the '--mockRobotManager' parameter to run robotDevastation.\n");
        return false;
    }

    RD_SUCCESS("Connected to remote robot.\n");

    return true;
}

bool rd::YarpRobotManager::disconnect()  {
    rpcClient.close();
    return true;
}

bool rd::YarpRobotManager::test()  {
    return true;
}

void rd::YarpRobotManager::setEnabled(bool enabled)
{

}

void rd::YarpRobotManager::onDestroy(){
    return;
}

bool rd::YarpRobotManager::send1vocab1int(int vocab, int integer)
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

bool rd::YarpRobotManager::send1vocab(int vocab)
{
    yarp::os::Bottle cmd, response;
    cmd.addVocab(vocab);
    rpcClient.write(cmd,response);
    if( response.get(0).asVocab() == VOCAB_OK )
        return true;
    else
        return false;
}

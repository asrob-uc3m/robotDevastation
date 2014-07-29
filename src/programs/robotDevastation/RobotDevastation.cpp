// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{

    //printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RobotDevastation options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--name\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("RobotDevastation using no additional special options.\n");

    if(rf.check("id"))
    {
        RD_INFO("id: %d\n",rf.find("id").asInt());
    }
    else
    {
        RD_ERROR("No id!\n");
        return false;
    }

    if(rf.check("name"))
    {
        RD_INFO("name: %s\n",rf.find("name").asString().c_str());
    }
    else
    {
        RD_ERROR("No name!\n");
        return false;
    }

    if(rf.check("team"))
    {
        RD_INFO("team: %d\n",rf.find("team").asInt());
    }
    else
    {
        RD_ERROR("No team!\n");
        return false;
    }

    RdPlayer rdPlayer(rf.find("id").asInt(),std::string(rf.find("name").asString()),100,rf.find("team").asInt());

    initSound();
    audioManager.playMusic("bso", -1);

    rateThreadOutput.setInImg(&inImg);    
    rateThreadOutput.init(rf);

    rateThreadProcess.setInImg(&inImg);    
    rateThreadProcess.init(rf);

    eventInput.start();   
    

    //-----------------OPEN LOCAL PORTS------------//
    std::ostringstream s;
    s << rdPlayer.getId();
    inImg.open(("/img/"+s.str()).c_str());
    rpcClient.open(("/rpc/"+s.str()).c_str());

    while(1){
        if(rpcClient.getOutputCount() > 0) break;
        printf("Waiting to be connected to server...\n");
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( ("/rpc/"+s.str()).c_str() , "/rdServer" );
    }

    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGIN);
    msgRdPlayer.addInt(rdPlayer.getId());
    msgRdPlayer.addString(rdPlayer.getName().c_str());
    msgRdPlayer.addInt(rdPlayer.getTeam_id());
    rpcClient.write(msgRdPlayer,res);
    RD_INFO("rdServer response from login: %s\n",res.toString().c_str());
    return true;
}

double rd::RobotDevastation::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RobotDevastation::updateModule()
{
    printf("RobotDevastation alive...\n");
    return true;
}

bool rd::RobotDevastation::initSound()
{
    if (audioManager.load("../../share/sounds/RobotDevastationBSO.mp3", "bso", 0) == -1)
        return false;

    if (audioManager.load("../../share/sounds/01_milshot.wav", "shoot", 1) == -1 )
        return false;

    if (audioManager.load("../../share/sounds/15_explosion.wav", "explosion", 1) == -1)
        return false;

    return true;
}

bool rd::RobotDevastation::interruptModule() {
    printf("RobotDevastation closing...\n");
    audioManager.destroy();
    inImg.interrupt();
    inImg.close();
    return true;
}


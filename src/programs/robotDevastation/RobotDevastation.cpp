// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

bool rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)
{

    //printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RobotDevastation options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--id integer\n");
        printf("\t--name string\n");
        printf("\t--team integer\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("RobotDevastation using no additional special options.\n");

    rdRoot = ::getenv ("RD_ROOT");
    if (rdRoot!=NULL)
    {
        RD_INFO("The RD_ROOT is: %s\n",rdRoot);
    }
    else
    {
        RD_WARNING("No RD_ROOT environment variable!\n");
        rdRoot="../..";  //-- Allow to run from build/bin
    }

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

    mentalMap.configure( rf.find("id").asInt() );

    std::vector< RdPlayer > players;
    players.push_back( RdPlayer(rf.find("id").asInt(),std::string(rf.find("name").asString()),100,100,rf.find("team").asInt(),0) );
    mentalMap.updatePlayers(players);

    if( ! initSound() )
        return false;

    audioManager.playMusic("bso", -1);

    rateThreadOutput.setRdRoot(rdRoot);
    rateThreadOutput.setMentalMap(&mentalMap);
    rateThreadOutput.setInImg(&inImg);
    rateThreadOutput.init(rf);

    rateThreadProcess.setMentalMap(&mentalMap);
    rateThreadProcess.setInImg(&inImg);
    rateThreadProcess.init(rf);

    eventInput.start();   
    
    callbackPort.setMentalMap(&mentalMap);

    //-----------------OPEN LOCAL PORTS------------//
    std::ostringstream s;
    s << mentalMap.getMyself().getId();
    inImg.open(("/img/"+s.str()).c_str());
    rpcClient.open(("/rpc/"+s.str()).c_str());
    callbackPort.open(("/callback/"+s.str()).c_str());
    callbackPort.useCallback();

    while(1){
        if(rpcClient.getOutputCount() > 0) break;
        printf("Waiting for rpc to be connected to server...\n");
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( ("/rpc/"+s.str()).c_str() , "/rdServer" );
    }
    yarp::os::Network::connect( "/rdBroadcast", ("/callback/"+s.str()).c_str() );

    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGIN);
    msgRdPlayer.addInt(mentalMap.getMyself().getId());
    msgRdPlayer.addString(mentalMap.getMyself().getName().c_str());
    msgRdPlayer.addInt(mentalMap.getMyself().getTeamId());
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
    printf("===robotDevastation===\n");
    printf("Number of players: %zd\n",mentalMap.getPlayers().size());
    for(size_t i=0;i<mentalMap.getPlayers().size();i++)
    {
       printf("----------------------\n%s\n",mentalMap.getPlayers().at(i).str().c_str());
    }
    //printf("======================\n");
    return true;
}

bool rd::RobotDevastation::initSound()
{
    std::string rdRootStr(rdRoot);

    if ( ! audioManager.load(rdRootStr+"/share/sounds/RobotDevastationBSO.mp3", "bso", 0) )
        return false;

    if ( ! audioManager.load(rdRootStr+"/share/sounds/01_milshot.wav", "shoot", 1) )
        return false;

    if ( ! audioManager.load(rdRootStr+"/share/sounds/15_explosion.wav", "explosion", 1) )
        return false;

    return true;
}

bool rd::RobotDevastation::interruptModule() {
    RD_INFO("Logout...\n");
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGOUT);
    msgRdPlayer.addInt(mentalMap.getMyself().getId());
    rpcClient.write(msgRdPlayer,res);
    RD_INFO("Closing program...\n");
    audioManager.destroy();
    callbackPort.disableCallback();
    // interrupt ports
    inImg.interrupt();
    callbackPort.interrupt();
    // close ports
    inImg.close();
    callbackPort.close();
    return true;
}


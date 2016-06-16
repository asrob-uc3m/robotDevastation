// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdServer.hpp"

bool rd::RdServer::configure(yarp::os::ResourceFinder &rf)
{
    rdRpcResponder.setPlayers(&players);
    rdBroadcast.open("/rdBroadcast");
    rpcServer.open("/rdServer");
    rpcServer.setReader(rdRpcResponder);
    return true;
}

rd::RdServer::~RdServer()
{

}

double rd::RdServer::getPeriod()
{
    return 0.1;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RdServer::updateModule()
{
    yarp::os::Bottle msgBroadcast;
    msgBroadcast.addVocab(VOCAB_RD_PLAYERS);
    printf("=======rdServer=======\n");
    printf("Number of players: %zd\n",players.size());

    typedef std::map<int, RdPlayer>::iterator it_type;
    for(it_type iterator = players.begin(); iterator != players.end(); iterator++)
    {
       printf("----------------------\n%s\n", (iterator->second).str().c_str());
       yarp::os::Bottle msgPlayer;
       msgPlayer.addInt( (iterator->second).getId() );
       msgPlayer.addString( (iterator->second).getName().c_str() );
       msgPlayer.addInt( (iterator->second).getHealth() );
       msgPlayer.addInt( (iterator->second).getMaxHealth() );
       msgPlayer.addInt( (iterator->second).getTeamId() );
       msgPlayer.addInt( (iterator->second).getScore() );
       msgBroadcast.addList() = msgPlayer;
    }

    //printf("======================\n");
    rdBroadcast.write(msgBroadcast);
    return true;
}

bool rd::RdServer::interruptModule() {
    printf("RdServer closing...\n");
    rdBroadcast.interrupt();
    rpcServer.interrupt();
    rdBroadcast.close();
    rpcServer.close();
    return true;
}


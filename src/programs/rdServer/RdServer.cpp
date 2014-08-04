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

double rd::RdServer::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RdServer::updateModule()
{
    yarp::os::Bottle msgBroadcast;
    msgBroadcast.addVocab(VOCAB_RD_PLAYERS);
    printf("=======rdServer=======\n");
    printf("Number of players: %zd\n",players.size());
    for(size_t i=0;i<players.size();i++)
    {
       printf("----------------------\n%s\n",players[i].str().c_str());
       yarp::os::Bottle msgPlayer;
       msgPlayer.addInt(players[i].getId());
       msgPlayer.addString(players[i].getName());
       msgPlayer.addInt(players[i].getHealth());
       msgPlayer.addInt(players[i].getMaxHealth());
       msgPlayer.addInt(players[i].getTeamId());
       msgPlayer.addInt(players[i].getScore());
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


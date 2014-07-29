// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdServer.hpp"

bool rd::RdServer::configure(yarp::os::ResourceFinder &rf)
{
    rdRpcResponder.setPlayers(&players);
    rpcServer.open("/rdServer");
    rpcServer.setReader(rdRpcResponder);
    printf("======================\n");
    return true;
}

double rd::RdServer::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::RdServer::updateModule()
{
    printf("Number of players: %zd\n",players.size());
    for(size_t i=0;i<players.size();i++)
    {
       printf("----------------------\n%s\n",players[i].str().c_str());
    }
    printf("======================\n");
    return true;
}



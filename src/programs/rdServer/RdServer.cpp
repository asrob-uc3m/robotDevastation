// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdServer.hpp"

bool rd::RdServer::configure(yarp::os::ResourceFinder &rf)
{
    rdRpcResponder.setPlayers(&players);
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
    RD_INFO("============\n");
    RD_INFO("Number of players: %zd\n",players.size());
    for(size_t i=0;i<players.size();i++)
       RD_INFO("%s\n----------",players[i].str().c_str());
    return true;
}



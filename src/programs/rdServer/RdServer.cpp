// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdServer.hpp"

bool rd::RdServer::configure(yarp::os::ResourceFinder &rf)
{
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
    printf("RdServer alive...\n");
    return true;
}



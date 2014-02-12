// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

#include "RdInputKeyboard.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"

namespace rdclient {

/************************************************************************/
RdClient::RdClient() { }

/************************************************************************/
bool RdClient::configure(yarp::os::ResourceFinder &rf) {

    watchdog = rf.check("watchdog",DEFAULT_WATCHDOG).asDouble();
    std::cout << "RdClient using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << std::endl;

    rdInputBasePtr = new rdlib::RdInputKeyboard();
    rdInputBasePtr->init();

    //&rdInputBasePtr->init();
    //void (classA::*memfun)(void);
    //printf("Hi: %p,\n",);
    rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();
    //(rdRobotBasePtr->getFunctionByName("shoot") )( (void*) rdRobotBasePtr);

    return executionThread.start();
}

/************************************************************************/

bool RdClient::updateModule() {
    std::cout << "RdClient alive..." << std::endl;
    return true;
}

/************************************************************************/

double RdClient::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool RdClient::interruptModule() {  // Closing rutines.
    delete rdInputBasePtr;
    rdInputBasePtr = 0;
    return true;
}

/************************************************************************/

} //rdclient


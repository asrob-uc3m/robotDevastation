// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

namespace rdclient {

/************************************************************************/
RdClient::RdClient() { }

/************************************************************************/
bool RdClient::configure(yarp::os::ResourceFinder &rf) {

    watchdog = rf.check("watchdog",DEFAULT_WATCHDOG).asDouble();
    cout << "RdClient using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << endl;

    /*watchdog = DEFAULT_WATCHDOG;  // double
    
    cout << "--------------------------------------------------------------" << endl;
    if (rf.check("help")) {
        cout << "RdClient Options:" << endl;
        cout << "\t--watchdog ([s] default: \"" << watchdog << "\")" << endl;
    }

    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    cout << "RdClient using watchdog [s]: " << watchdog << "." << endl;*/

    

    return executionThread.init(rf);
}

/************************************************************************/
bool RdClient::updateModule() {
    cout << "RdClient alive..." << endl;
    return true;
}

/************************************************************************/
double RdClient::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/
bool RdClient::interruptModule() {
    return true;
}

/************************************************************************/

} //rdclient


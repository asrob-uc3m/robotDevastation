// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

#include "RdInputKeyboard.hpp"
#include "RdOutputHighgui.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"
#include "RdManagerDefault.hpp"

namespace rdclient {

/************************************************************************/
RdClient::RdClient() { }

/************************************************************************/
bool RdClient::runProgram() {

    watchdog = DEFAULT_WATCHDOG;
    std::cout << "RdClient using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << std::endl;

    rdManagerBasePtr = new rdlib::RdManagerDefault();

    rdInputBasePtr = new rdlib::RdInputKeyboard();
    rdOutputBasePtr = new rdlib::RdOutputHighgui();
    rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();

    rdManagerBasePtr->setRdInputBasePtr(rdInputBasePtr);
    rdManagerBasePtr->setRdRobotBasePtr(rdRobotBasePtr);

    rdInputBasePtr->setRdManagerBasePtr(rdManagerBasePtr);

    while(1) {
        std::cout << "RdClient alive..." << std::endl;
        usleep( watchdog * 1000000.0 );
    }

    return true;
}

/************************************************************************/

bool RdClient::closeProgram() {  // Closing rutines.
    delete rdInputBasePtr;
    rdInputBasePtr = 0;
    return true;
}

/************************************************************************/

} //rdclient


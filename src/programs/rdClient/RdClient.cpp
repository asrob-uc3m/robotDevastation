// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

#include "RdInputKeyboard.hpp"
#include "RdOutputHighgui.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"
#include "RdManagerDefault.hpp"

namespace rdclient {

/************************************************************************/
RdClient::RdClient() {
    std::cout << "[info] RdClient constructor." << std::endl;
}

/************************************************************************/
RdClient::~RdClient() {
    std::cout << "[info] RdClient destructor." << std::endl;
}

/************************************************************************/
bool RdClient::runProgram() {

    rdManagerBasePtr = 0;
    rdInputBasePtr = 0;
    rdOutputBasePtr = 0;
    rdRobotBasePtr = 0;

    watchdog = DEFAULT_WATCHDOG;
    std::cout << "[info] RdClient using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << std::endl;

    rdManagerBasePtr = new rdlib::RdManagerDefault();
    rdInputBasePtr = new rdlib::RdInputKeyboard();
    //rdOutputBasePtr = new rdlib::RdOutputHighgui();  // still in development
    rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();

    rdManagerBasePtr->setRdInputBasePtr(rdInputBasePtr);
    rdManagerBasePtr->setRdRobotBasePtr(rdRobotBasePtr);

    rdInputBasePtr->setRdManagerBasePtr(rdManagerBasePtr);

    /*std::cout << "[info] RdClient quit in 1 second..." << std::endl;
    usleep( 1 * 1000000.0 );
    quitProgram();*/

    while(1) {
        std::cout << "RdClient alive..." << std::endl;
        usleep( watchdog * 1000000.0 );
    }

    return true;
}

/************************************************************************/

bool RdClient::quitProgram() {  // Closing rutines.
    std::cout << "[info] RdClient quitProgram()" << std::endl;
    if (rdInputBasePtr) {
        rdInputBasePtr->quit();
        delete rdInputBasePtr;
        rdInputBasePtr = 0;
    }
    if (rdOutputBasePtr) {
        rdOutputBasePtr->quit();
        delete rdOutputBasePtr;
        rdOutputBasePtr = 0;
    }
    if (rdRobotBasePtr) {
        rdRobotBasePtr->quit();
        delete rdRobotBasePtr;
        rdRobotBasePtr = 0;
    }
    if (rdManagerBasePtr) {
        rdManagerBasePtr->quit();
        delete rdManagerBasePtr;
        rdManagerBasePtr = 0;
    }
    return true;
}

/************************************************************************/

} //rdclient


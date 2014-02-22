// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

//#include "RdInputKeyboard.hpp"
//#include "RdOutputHighgui.hpp"
  #include "RdInOutHighgui.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"
#include "RdManagerDefault.hpp"
#include "RdCameraWebcam.hpp"

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
    rdCameraBasePtr = 0;

    watchdog = DEFAULT_WATCHDOG;
    std::cout << "[info] RdClient using watchdog [s]: " << watchdog << " (default: " << DEFAULT_WATCHDOG << ")." << std::endl;

    rdCameraBasePtr = new rdlib::RdCameraWebcam();
    rdManagerBasePtr = new rdlib::RdManagerDefault();
    //rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();

    //-- Use the next TWO lines for one input and a separated output
    //rdInputBasePtr = new rdlib::RdInputKeyboard();
    //rdOutputBasePtr = new rdlib::RdOutputHighgui();
    //-- OR use the NEXT THREE lines for one input+output
    rdlib::RdInOutHighgui * rdInOutHighguiPtr = new rdlib::RdInOutHighgui();
    rdOutputBasePtr = rdInOutHighguiPtr;
    rdInputBasePtr = rdInOutHighguiPtr;

    rdManagerBasePtr->setRdCameraBasePtr(rdCameraBasePtr);
    rdManagerBasePtr->setRdRobotBasePtr(rdRobotBasePtr);

    //-- Use the next TWO lines for one input and a separated output
    //rdManagerBasePtr->setRdInputBasePtr(rdInputBasePtr);
    //rdManagerBasePtr->setRdOutputBasePtr(rdOutputBasePtr);
    //-- OR use the NEXT TWO lines for one input+output
    rdManagerBasePtr->setRdInputBasePtr(rdInOutHighguiPtr);
    rdManagerBasePtr->setRdOutputBasePtr(rdInOutHighguiPtr);

    //-- Use the next TWO lines for one input and a separated output
    //rdInputBasePtr->setRdManagerBasePtr(rdManagerBasePtr);
    //rdOutputBasePtr->setRdManagerBasePtr(rdManagerBasePtr);
    //-- OR use the NEXT ONE lines for one input+output
    rdInOutHighguiPtr->setRdManagerBasePtr(rdManagerBasePtr);

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
    if (rdCameraBasePtr) {
        rdCameraBasePtr->quit();
        delete rdCameraBasePtr;
        rdCameraBasePtr = 0;
    }
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


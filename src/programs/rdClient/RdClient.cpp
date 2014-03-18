// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdClient.hpp"

#include "RdInputHighgui.hpp"
#include "RdOutputHighgui.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"
#include "RdManagerDefault.hpp"
#include "RdCameraWebcam.hpp"

rdclient::RdClient* globalRdClient;
void rdclient::RdClient::staticSignalHandler(int s)
{
    //-- see "kill -l" for a list of meanings. 2 is a SIGINT (ctrl-c).
    std::string msg("Caught signal ");
    if (s==2)
        msg += "SIGINT (usually a CTRL-C)";
    else if (s==15)
        msg += "SIGTERM";
    else
        msg += "Unknown";
    RD_INFO("%s. Bye!\n",msg.c_str());
    globalRdClient->getRdManagerBasePtr()->askToStop();
}

bool rdclient::RdClient::runProgram()
{
    rdManagerBasePtr = 0;
    rdInputBasePtr = 0;
    rdOutputBasePtr = 0;
    rdRobotBasePtr = 0;
    rdCameraBasePtr = 0;

    watchdog = DEFAULT_WATCHDOG;

    //-- .ini reader
    if(!getenv("RD_ROOT"))
    {
        RD_ERROR("check your RD_ROOT environomental variable, it is empty!\n");
        return -1;
    }
    RD_INFO("Using RD_ROOT environomental variable: %s\n",getenv("RD_ROOT"));
    std::string inifilename( getenv("RD_ROOT") );
    inifilename += "/share/rdClient/conf/rdClient.ini";
    rdlib::RdIniReader< std::string, std::string > rdIniReader;
    rdIniReader.parseFile(inifilename.c_str(), rdIniMap);
    RD_INFO("rdIniMap contains:\n");
    for (std::map< std::string, std::string >::iterator it = rdIniMap.begin(); it != rdIniMap.end(); ++it)
        RD_INFO("%s => %s\n", it->first.c_str(), it->second.c_str());
    RD_INFO("end rdIniMap contains.\n");

    RD_INFO("RdClient using watchdog [s]: %f (default: \"%f\").\n", watchdog, DEFAULT_WATCHDOG);

    //-- Set up callbacks
    globalRdClient = this;
    signal (SIGINT,RdClient::staticSignalHandler);
    signal (SIGTERM,RdClient::staticSignalHandler);

    rdCameraBasePtr = new rdlib::RdCameraWebcam();
    rdManagerBasePtr = new rdlib::RdManagerDefault();
    rdRobotBasePtr = new rdlib::RdRobotLaserTowerOfDeath();

    rdOutputBasePtr = new rdlib::RdOutputHighgui();
    rdInputBasePtr = new rdlib::RdInputHighgui();

    //-- Link manager to the rest of the modules:
    rdManagerBasePtr->setRdCameraBasePtr(rdCameraBasePtr);
    rdManagerBasePtr->setRdRobotBasePtr(rdRobotBasePtr);
    rdManagerBasePtr->setRdInputBasePtr(rdInputBasePtr);
    rdManagerBasePtr->setRdOutputBasePtr(rdOutputBasePtr);

    //-- Setup the manager
    rdManagerBasePtr->setup();

    //-- This is only needed because we are using OpenCV's Highgui I/O
    ((rdlib::RdInputHighgui *) rdInputBasePtr)->setRdOutputHighguiPtr( (rdlib::RdOutputHighgui*) rdOutputBasePtr);

    //-- Pass reference to the commands/actions
    rdRobotBasePtr->setRdIniMap(rdIniMap);

    //-- Start components
    rdCameraBasePtr->start();
    rdManagerBasePtr->start();
    rdOutputBasePtr->start();

    rdRobotBasePtr->connect(); //! \todo Change this to setup
    rdInputBasePtr->start();

    int managerStatus;
    while(1)
    {
        managerStatus = rdManagerBasePtr->getManagerStatus();
        RD_INFO("RdClient alive, managerStatus: %d\n",managerStatus);
        if (managerStatus == rdlib::RdManagerBase::MANAGER_STATUS_STOPPED)
            break;
        usleep( watchdog * 1000000.0 );
    }
    return this->quitProgram();
}

//-- Closing rutines.
bool rdclient::RdClient::quitProgram()
{
    RD_INFO("Stopping components...\n");
    if (rdInputBasePtr)
        rdInputBasePtr->askToStop();

    if (rdRobotBasePtr)
        rdRobotBasePtr->askToStop();

    if (rdManagerBasePtr)
        rdManagerBasePtr->askToStop();

    usleep( 1e5);

    RD_INFO("Deleting components...\n");
    if (rdInputBasePtr) {
        rdInputBasePtr->quit();
        delete rdInputBasePtr;
        rdInputBasePtr = 0;
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

    RD_SUCCESS("RdClient quitProgram(): quit gracefully, bye!\n");
    return true;
}

rdlib::RdManagerBase *rdclient::RdClient::getRdManagerBasePtr()
{
    return rdManagerBasePtr;
}

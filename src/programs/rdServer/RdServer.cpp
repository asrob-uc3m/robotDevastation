// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdServer.hpp"

#include "RdInputHighgui.hpp"
#include "RdOutputHighgui.hpp"
#include "RdRobotLaserTowerOfDeath.hpp"
#include "RdManagerDefault.hpp"
#include "RdCameraWebcam.hpp"

rdserver::RdServer* globalRdServer;
void rdserver::RdServer::staticSignalHandler(int s)
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
    globalRdServer->quitProgram();
    //globalRdServer->getRdManagerBasePtr()->askToStop();
}

bool rdserver::RdServer::runProgram(const int& argc, char *argv[])
{

    watchdog = DEFAULT_WATCHDOG;

    //-- .ini reader
    if(!getenv("RD_ROOT"))
    {
        RD_ERROR("Required RD_ROOT environomental variable empty!\n");
        RD_ERROR("Include the line \"export RD_ROOT=/your/path/to/robotDevastation\" or similar in .bashrc or .profile\n");
        return -1;
    }
    RD_INFO("Using RD_ROOT environomental variable: %s\n",getenv("RD_ROOT"));
    std::string inifilename( getenv("RD_ROOT") );
    inifilename += "/share/rdServer/conf/rdServer.ini";
    rdlib::RdIniReader< std::string, std::string > rdIniReader;
    rdIniReader.parseFile(inifilename.c_str(), rdIniMap);
    RD_INFO("rdIniMap contains:\n");
    for (std::map< std::string, std::string >::iterator it = rdIniMap.begin(); it != rdIniMap.end(); ++it)
        RD_INFO("%s => %s\n", it->first.c_str(), it->second.c_str());
    RD_INFO("end rdIniMap contains.\n");

    RD_INFO("RdServer using watchdog [s]: %f (default: \"%f\").\n", watchdog, DEFAULT_WATCHDOG);

    //-- Set up callbacks
    globalRdServer = this;
    signal (SIGINT,RdServer::staticSignalHandler);
    signal (SIGTERM,RdServer::staticSignalHandler);


    serverStatus = 0;
    while(serverStatus == 0)
    {
        RD_INFO("RdServer alive, serverStatus: %d\n",serverStatus);
        usleep( watchdog * 1000000.0 );
    }
    return this->quitProgram();
}

//-- Closing rutines.
bool rdserver::RdServer::quitProgram()
{
    RD_INFO("Stopping components...\n");
    serverStatus = -1;

    RD_INFO("Deleting components...\n");

    RD_SUCCESS("Quit gracefully, bye!\n");
    return true;
}


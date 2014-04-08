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

    if( ! this->createPort() ) {
        RD_ERROR("Could not create port.\n");
        return false;
    }

    int res = listen(sockfd, 10);
    if ( res != 0   ) { //-- Careful, max 10 connections.
        RD_ERROR("Could not listen.\n");
        return false;
    }


    vectorOfScores.resize(2);

    serverStatus = 0;
    while(serverStatus == 0)
    {
        usleep( watchdog * 1000000.0 );
        RD_INFO("RdServer alive, serverStatus: %d\n",serverStatus);
        for(int i=0;i<vectorOfScores.size();i++){
            RD_INFO("Player %d: %d.\n",i+1,vectorOfScores[i]);
        }

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

bool rdserver::RdServer::createPort()
{
    struct sockaddr_in my_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        RD_ERROR("sockfd failed.\n");
        return false;
    }
    else
        RD_SUCCESS("sockfd ok.\n");
    //-- host byte order
    my_addr.sin_family = AF_INET;
    //-- short, network byte order
    my_addr.sin_port = htons(DEFAULT_PORT);
    //my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
    //-- zero the rest of the struct
    memset(&(my_addr.sin_zero), 0, 8);
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        RD_ERROR("bind() fail.\n");
        return false;
    }
    else
        RD_SUCCESS("bind() ok.\n");
    return true;
}

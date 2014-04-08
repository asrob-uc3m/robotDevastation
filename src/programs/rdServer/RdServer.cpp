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

    //struct sockaddr_in their_addr;
    //int sin_size = sizeof their_addr;
    //int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);


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
    // [thanks] http://wiki.treck.com/Introduction_to_BSD_Sockets

    //-- Specify the address family
    destAddr.sin_family = AF_INET;
    //-- Specify the IP
    destAddr.sin_addr.s_addr = inet_addr(DEFAULT_IP);  // Alternatively use INADDR_ANY
    //-- Specify the port
    destAddr.sin_port = htons(DEFAULT_PORT);
    //-- zero the rest of the struct
    memset(&(destAddr.sin_zero), 0, 8);

    //-- Create a socket
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // Alternatively use 0 as last argument
    //-- Make sure the socket was created successfully
    if(listenSocket  == -1)
    {
        RD_ERROR("sockfd failed.\n");
        return false;
    }
    RD_SUCCESS("sockfd ok.\n");

    //-- Bind the socket to the port and address at which we wish to receive data
    if( bind(listenSocket, (struct sockaddr *)&destAddr, sizeof(struct sockaddr)) == -1 )
    {
        RD_ERROR("bind fail.\n");
        return false;
    }
    RD_SUCCESS("bind ok.\n");

    //-- Set up the socket as a listening socket
    int res = listen(listenSocket, 10);  //-- Careful, max 10 connections.
    if ( res < 0   ) {
        RD_ERROR("Could not listen.\n");
        return false;
    }
    RD_SUCCESS("listen ok.\n");

    RD_INFO("Accepting clients...\n");
    socklen_t addrLen = sizeof(sourceAddr);
    newSocket = accept(listenSocket, (struct sockaddr *)&sourceAddr, &addrLen);

    return true;
}

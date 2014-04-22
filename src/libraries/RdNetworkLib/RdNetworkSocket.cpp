// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdNetworkSocket.hpp"

bool rdlib::RdNetworkSocket::setup()
{
    RD_DEBUG("\n");

    if(!getenv("RD_ROOT"))
    {
        RD_ERROR("Required RD_ROOT environomental variable empty!\n");
        RD_ERROR("Include the line \"export RD_ROOT=/your/path/to/robotDevastation\" or similar in .bashrc or .profile\n");
        return false;
    }

    //-- Specify the address family
    destAddr.sin_family = AF_INET;
    //-- Specify the IP
    destAddr.sin_addr.s_addr = inet_addr(DEFAULT_IP);  // Alternatively use INADDR_ANY
    //-- Specify the port
    destAddr.sin_port = htons(DEFAULT_PORT);
    //-- zero the rest of the struct
    memset(&(destAddr.sin_zero), 0, 8);

    //-- Create a socket
    testSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // Alternatively use 0 as last argument
    //-- Make sure the socket was created successfully
    if(testSocket  == -1)
    {
        RD_ERROR("sockfd failed.\n");
        return false;
    }
    RD_SUCCESS("sockfd ok.\n");

    int res = connect(testSocket, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if ( res < 0   ) {
        RD_ERROR("could not connect.\n");
        return false;
    }
    RD_SUCCESS("ok connect.\n");

    return true;
}

bool rdlib::RdNetworkSocket::pointerIncrement() {
    return true;
}

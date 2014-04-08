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

    return true;
}

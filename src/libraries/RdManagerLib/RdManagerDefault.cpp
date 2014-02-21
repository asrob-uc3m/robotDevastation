// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

namespace rdlib{

/************************************************************************/

bool RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

/************************************************************************/

RdManagerDefault::RdManagerDefault() {
    std::cout << "[info] RdManagerDefault constructor." << std::endl;
}

/************************************************************************/

RdManagerDefault::~RdManagerDefault() {
    std::cout << "[info] RdManagerDefault destructor." << std::endl;
}

/************************************************************************/

bool RdManagerDefault::quit() {
    return true;
}


/************************************************************************/

} //rdlib


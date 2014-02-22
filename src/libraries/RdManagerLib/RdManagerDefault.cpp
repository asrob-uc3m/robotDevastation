// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

bool rdlib::RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

rdlib::RdManagerDefault::RdManagerDefault()
{
    std::cout << "[info] RdManagerDefault constructor." << std::endl;
    this->managerStatus = 0;
}

bool rdlib::RdManagerDefault::quit()
{
    std::cout << "[info] RdManagerDefault quit." << std::endl;
    this->managerStatus = -1;
    return true;
}

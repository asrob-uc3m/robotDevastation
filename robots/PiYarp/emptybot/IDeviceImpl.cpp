// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EmptyBot.hpp"

// ------------------- DeviceDriver Related ------------------------------------

bool EmptyBot::open(Searchable& config) {

    numMotors = config.check("pwmPins",Value(DEFAULT_NUM_MOTORS),"Number of motors").asInt();

    //Bottle ids = config.findGroup("ids").tail();  //-- e.g. 15
    
    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::close() {

    return true;
}

// -----------------------------------------------------------------------------


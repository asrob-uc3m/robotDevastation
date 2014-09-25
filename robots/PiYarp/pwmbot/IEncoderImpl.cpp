// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PwmBot.hpp"

// ------------------ IEncoder Related -----------------------------------------

bool PwmBot::resetEncoder(int j) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::resetEncoders() {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::setEncoder(int j, double val) {  // encExposed = val;
    RD_INFO("(%d,%f)\n",j,val);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::setEncoders(const double *vals) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= setEncoder(i,vals[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoder(int j, double *v) {
    //RD_INFO("%d\n",j);  //-- Too verbose in stream.

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoders(double *encs) {
    //RD_INFO("\n");  //-- Too verbose in stream.

    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= getEncoder(i,&encs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoderSpeed(int j, double *sp) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoderSpeeds(double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getEncoderSpeed(i,&spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoderAcceleration(int j, double *spds) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::getEncoderAccelerations(double *accs) {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------


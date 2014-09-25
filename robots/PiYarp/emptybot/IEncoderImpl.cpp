// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EmptyBot.hpp"

// ------------------ IEncoder Related -----------------------------------------

bool EmptyBot::resetEncoder(int j) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::resetEncoders() {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setEncoder(int j, double val) {  // encExposed = val;
    RD_INFO("(%d,%f)\n",j,val);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setEncoders(const double *vals) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= setEncoder(i,vals[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoder(int j, double *v) {
    //RD_INFO("%d\n",j);  //-- Too verbose in stream.

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoders(double *encs) {
    //RD_INFO("\n");  //-- Too verbose in stream.

    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= getEncoder(i,&encs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoderSpeed(int j, double *sp) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoderSpeeds(double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getEncoderSpeed(i,&spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoderAcceleration(int j, double *spds) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getEncoderAccelerations(double *accs) {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------


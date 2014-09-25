// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EmptyBot.hpp"

// ------------------ IPosition Related ----------------------------------------

bool EmptyBot::getAxes(int *axes) {
    *axes = numMotors;
    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setPositionMode() {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::positionMove(int j, double ref) {  // encExposed = ref;
    RD_INFO("(%d,%f)\n",j,ref);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::positionMove(const double *refs) {  // encExposed = refs;
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::relativeMove(int j, double delta) {
    RD_INFO("(%d, %f)\n",j,delta);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::relativeMove(const double *deltas) {  // encExposed = deltas + encExposed

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::checkMotionDone(int j, bool *flag) {
    RD_INFO("(%d)\n",j);

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::checkMotionDone(bool *flag) {
    RD_INFO("\n");

    bool ok = true;
    for(int j=0; j<numMotors; j++)
    {
        ok &= this->checkMotionDone(j,&flag[j]);
    }
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setRefSpeed(int j, double sp) {
    RD_INFO("(%d, %f)\n",j,sp);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setRefSpeeds(const double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= setRefSpeed(i,spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setRefAcceleration(int j, double acc) {
    RD_INFO("(%d, %f)\n",j,acc);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::setRefAccelerations(const double *accs) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= setRefAcceleration(i,accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getRefSpeed(int j, double *ref) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getRefSpeeds(double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getRefSpeed(i,&spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getRefAcceleration(int j, double *acc) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::getRefAccelerations(double *accs) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getRefAcceleration(i,&accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool EmptyBot::stop(int j) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool EmptyBot::stop() {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= stop(i);
    return ok;
}

// -----------------------------------------------------------------------------


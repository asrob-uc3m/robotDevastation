// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PwmBot.hpp"

// ------------------ IPosition Related ----------------------------------------

bool PwmBot::getAxes(int *axes) {
    *axes = gpios.size();
    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::setPositionMode() {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::positionMove(int j, double ref) {  // encExposed = ref;
    RD_INFO("(%d,%f)\n",j,ref);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::positionMove(const double *refs) {  // encExposed = refs;
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::relativeMove(int j, double delta) {
    RD_INFO("(%d, %f)\n",j,delta);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::relativeMove(const double *deltas) {  // encExposed = deltas + encExposed

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::checkMotionDone(int j, bool *flag) {
    RD_INFO("(%d)\n",j);

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::checkMotionDone(bool *flag) {
    RD_INFO("\n");

    bool ok = true;
    for(int j=0; j<gpios.size(); j++)
    {
        ok &= this->checkMotionDone(j,&flag[j]);
    }
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::setRefSpeed(int j, double sp) {
    RD_INFO("(%d, %f)\n",j,sp);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::setRefSpeeds(const double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<gpios.size();i++)
        ok &= setRefSpeed(i,spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::setRefAcceleration(int j, double acc) {
    RD_INFO("(%d, %f)\n",j,acc);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::setRefAccelerations(const double *accs) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<gpios.size();i++)
        ok &= setRefAcceleration(i,accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::getRefSpeed(int j, double *ref) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::getRefSpeeds(double *spds) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<gpios.size();i++)
        ok &= getRefSpeed(i,&spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::getRefAcceleration(int j, double *acc) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::getRefAccelerations(double *accs) {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<gpios.size();i++)
        ok &= getRefAcceleration(i,&accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool PwmBot::stop(int j) {
    RD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( ! this->indexWithinRange(j) ) return false;

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::stop() {
    RD_INFO("\n");

    bool ok = true;
    for(unsigned int i=0;i<gpios.size();i++)
        ok &= stop(i);
    return ok;
}

// -----------------------------------------------------------------------------


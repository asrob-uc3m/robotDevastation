// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PwmBot.hpp"

// ------------------ IVelocity Related ----------------------------------------

bool PwmBot::setVelocityMode() {
    RD_INFO("\n");

    RD_WARNING("Not implemented yet.\n");

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::velocityMove(int j, double sp) {
    RD_INFO("(%d)\n",j);

    // Clear and start again
    clear_channel_gpio(0, gpios[j]);
    add_channel_pulse(0, gpios[j], 0, sp / PULSE_WIDTH_INCREMENT_GRANULARITY_US_DEFAULT);

    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::velocityMove(const double *sp) {
    RD_INFO("\n");

    bool ok = true;
    for(int j=0; j<gpios.size(); j++)
    {
        ok &= this->velocityMove(j,sp[j]);
    }
    return ok;
}

// -----------------------------------------------------------------------------


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PwmBot.hpp"

// ------------------- DeviceDriver Related ------------------------------------

bool PwmBot::open(Searchable& config) {

    numMotors = config.check("pwmPins",Value(DEFAULT_NUM_MOTORS),"Number of motors").asInt();

    //Bottle ids = config.findGroup("ids").tail();  //-- e.g. 15

    if ( config.check("pcm") )
    {
        setup(PULSE_WIDTH_INCREMENT_GRANULARITY_US_DEFAULT, DELAY_VIA_PCM);
        RD_INFO("Using pcm.\n");
    }
    else
    {
        setup(PULSE_WIDTH_INCREMENT_GRANULARITY_US_DEFAULT, DELAY_VIA_PWM);
        RD_INFO("Using std pwm.\n");
    }
    
    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::close() {

    return true;
}

// -----------------------------------------------------------------------------


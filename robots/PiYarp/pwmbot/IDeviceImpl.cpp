// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PwmBot.hpp"

// ------------------- DeviceDriver Related ------------------------------------

bool PwmBot::open(Searchable& config) {

    Bottle gpiosBottle = config.findGroup("gpios").tail();  //-- e.g. 17 27

    printf(BOLDBLUE);
    printf("PwmBot options:\n");
    printf("\t--gpios %s\n",gpiosBottle.toString().c_str());
    printf(RESET);

    if ( config.check("pcm") )
        setup(PULSE_WIDTH_INCREMENT_GRANULARITY_US_DEFAULT, DELAY_VIA_PCM);
    else
        setup(PULSE_WIDTH_INCREMENT_GRANULARITY_US_DEFAULT, DELAY_VIA_PWM);

    for(int j=0; j < gpiosBottle.size(); j++)
    {
        int gpio = gpiosBottle.get(j).asInt();
        init_channel(j, SUBCYCLE_TIME_US_DEFAULT);  //10ms;
        print_channel(j);
        add_channel_pulse(j, gpio, 0, 0);
        gpios.push_back( gpiosBottle.get(j).asInt() );
        RD_SUCCESS("Configured gpio %d on channel %d.\n",gpio,j);
    }
    
    return true;
}

// -----------------------------------------------------------------------------

bool PwmBot::close() {

    return true;
}

// -----------------------------------------------------------------------------


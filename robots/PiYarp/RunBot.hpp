// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RUN_BOT__
#define __RUN_BOT__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>

#include "RdMacros.hpp"

#define DEFAULT_TYPE "pwmbot"
#define DEFAULT_PREFIX "raspi"

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup RunBot
 *
 * The RunBot class tests the Pwm class as a controlboard.
 * 
 */
class RunBot : public RFModule {
protected:
    yarp::dev::PolyDriver robotDevice;

//    double getPeriod();
    bool updateModule();
//    bool interruptModule();
//    int period;

public:
    RunBot();
    bool configure(ResourceFinder &rf);
};

#endif  // __RUN_BOT__


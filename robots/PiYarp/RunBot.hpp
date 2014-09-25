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
#define DEFAULT_CAMERA "off"

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup RunBot
 *
 * The RunBot class tests the Pwm class as a controlboard.
 * 
 */
class RunBot : public RFModule {

    public:
        RunBot();
        bool configure(ResourceFinder &rf);

    protected:
        yarp::dev::PolyDriver cameraDevice;
        yarp::dev::PolyDriver robotDevice;

        virtual bool updateModule();
        virtual bool close();

//        double getPeriod();
//        bool interruptModule();
//        int period;

        std::string camera;

};

#endif  // __RUN_BOT__


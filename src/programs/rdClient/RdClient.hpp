// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 ASROB (http://asrob.uc3m.es)
  * This file is part of Robot Devastation Game
  * Author: Santiago Morante
  * Co-Author: Juan G. Victores
  */

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/all.h>

#include "GameControl.hpp"
#include "ScreenDraw.hpp"
#include "ExecutionThread.hpp"

#define DEFAULT_WATCHDOG    5       // [s]

namespace rdclient{

/**
 * @ingroup rdClient
 * @brief The main \ref rdClient program class, implemented as a slow watchdog class that also launches an \ref ExecutionThread.
 */
class RdClient : public yarp::os::RFModule {
    public:
        /**
         * Constructor og this slow watchdog thread.
         */
        RdClient();

        /**
         * Configure the module, pass a ResourceFinder object to the module.
         *
         * @param rf a previously initialized ResourceFinder
         * @return true/false upon success/failure
         */
        bool configure(yarp::os::ResourceFinder &rf);

    protected:

        /**
         * An instance of the main periodical fast thread.
         */
        ExecutionThread executionThread;

        /**
         * When your module wants to stop, return false.  The module's actual
         * work could be done during this call, or it could just check the
         * state of a thread running in the background.
         *
         * @return true if module should continue
        */
        bool updateModule();

        /**
         * Closing rutines.
         */
        bool interruptModule();
        
        /**
         * Tell YARP the period of this slow watchdog thread.
         */
        double getPeriod();


        /**
         * Will be returned through getPeriod().
         */
        double watchdog; // [s]


};

} //rdclient

#endif  // __ROBOT_DEVASTATION_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CALLBACK_PORT__
#define __CALLBACK_PORT__

#include <vector>

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>

#include "RdMacros.hpp"
#include "RdPlayer.hpp"
#include "RdVocabs.hpp"
#include "RdMentalMap.hpp"

namespace rd
{

/**
 * @ingroup CallbackPort
 *
 * CallbackPort class implements a port with callbacks.
 */
class CallbackPort : public yarp::os::BufferedPort<yarp::os::Bottle> {
    protected:
        /**
        * Implement the actual callback.
        */
        void onRead(yarp::os::Bottle& b);

        RdMentalMap* mentalMap;


    public:

        void setMentalMap(RdMentalMap *value);

};

}

#endif  // __CALLBACK_PORT__


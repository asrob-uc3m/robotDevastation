// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CALLBACK_PORT__
#define __CALLBACK_PORT__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include "RdMacros.hpp"

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


    public:

};

}

#endif  // __CALLBACK_PORT__


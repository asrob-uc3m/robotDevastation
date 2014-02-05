// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __EXECUTION_THREAD_HPP__
#define __EXECUTION_THREAD_HPP__

#include <iostream>

#include <yarp/os/RateThread.h>

#define DEFAULT_MS      20.0        // [ms]

namespace rdclient {

/**
 * @brief The fast periodic thread, e.g. controls video stream reception and screen refresh. Launched by \ref RdClient.
 */
class ExecutionThread : public yarp::os::RateThread {

    public:
    /** Constructor. */
    ExecutionThread() : RateThread(DEFAULT_MS) {}  // In ms

    /** Periodical loop function. This is the thread itself. */
    virtual void run();

};

} //rdclient

#endif  // __EXECUTION_THREAD_HPP__


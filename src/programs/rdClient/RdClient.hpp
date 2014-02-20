// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include "RdManagerBase.hpp"

#include "RdInputBase.hpp"
#include "RdOutputBase.hpp"
#include "RdRobotBase.hpp"

#define DEFAULT_WATCHDOG    5       // [s]

namespace rdclient{

/**
 * @ingroup rdClient
 * @brief The main \ref rdClient program class.
 */
class RdClient {

    public:
        /**
         * Constructor of this slow watchdog main thread.
         */
        RdClient();

        /**
         * Run the program.
         *
         * @return true/false upon success/failure
         */
        bool runProgram();

        /**
         * Close the program.
         *
         * @return true/false upon success/failure
         */
        bool closeProgram();

    protected:

        /** Watchdog period [s]. */
        double watchdog;

        /** Manager */
        rdlib::RdManagerBase* rdManagerBasePtr;

        /** Input */
        rdlib::RdInputBase* rdInputBasePtr;

        /** Output */
        rdlib::RdOutputBase* rdOutputBasePtr;

        /** Robot */
        rdlib::RdRobotBase* rdRobotBasePtr;

};

} //rdclient

#endif  // __ROBOT_DEVASTATION_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <signal.h>

#include "RdIniReader.hpp"
#include "RdMacros.hpp"

#include "RdManagerBase.hpp"

#include "RdInputBase.hpp"
#include "RdOutputBase.hpp"
#include "RdRobotBase.hpp"
#include "RdCameraBase.hpp"
#include "RdNetworkBase.hpp"

#define DEFAULT_WATCHDOG    1.0       // [s]

namespace rdclient{

/**
 * @ingroup rdClient
 * @brief The main \ref rdClient program class.
 */
class RdClient {

    public:
        /**
         * Run the program.
         *
         * @return true/false upon success/failure
         */
        bool runProgram(const int& argc, char *argv[]);

        /**
         * Quit the program.
         *
         * @return true/false upon success/failure
         */
        bool quitProgram();

        rdlib::RdManagerBase* getRdManagerBasePtr();

    protected:

        static void staticSignalHandler(int arg);

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

        /** Camera */
        rdlib::RdCameraBase* rdCameraBasePtr;

        /** Network */
        rdlib::RdNetworkBase* rdNetworkBasePtr;

        /** Map read from ini file */
        std::map< std::string, std::string > rdIniMap;

        /** Map of keys to functions */
        std::map< char, std::string > keyFunctionMap;

};

} //rdclient

#endif  // __ROBOT_DEVASTATION_HPP__


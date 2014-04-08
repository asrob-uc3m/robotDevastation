// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SERVER_HPP__
#define __RD_SERVER_HPP__

#include <signal.h>

#include "RdIniReader.hpp"
#include "RdMacros.hpp"

#define DEFAULT_WATCHDOG    1.0       // [s]

namespace rdserver{

/**
 * @ingroup rdServer
 * @brief The main \ref rdServer program class.
 */
class RdServer {

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

    protected:

        static void staticSignalHandler(int arg);

        /** Watchdog period [s]. */
        double watchdog;

        /** Map read from ini file */
        std::map< std::string, std::string > rdIniMap;

        int serverStatus;

};

} //rdserver

#endif  // __RD_SERVER_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SERVER_HPP__
#define __RD_SERVER_HPP__

#include <signal.h>

#include "RdIniReader.hpp"
#include "RdMacros.hpp"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>  // inet_addr in linux.
#include <netinet/in.h>  // sockaddr_in in linux.
#include <arpa/inet.h>

#define DEFAULT_WATCHDOG    1.0       // [s]
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 5000

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

        bool createPort();

        /** Socket file descriptor */
        int sockfd;

        std::vector<int> vectorOfScores;
};

} //rdserver

#endif  // __RD_SERVER_HPP__


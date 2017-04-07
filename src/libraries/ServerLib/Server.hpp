// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SERVER_HPP__
#define __RD_SERVER_HPP__

#include <map>

#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Mutex.h>
#include <yarp/os/Port.h>

#include "Player.hpp"
#include "RpcResponder.hpp"

#define DEFAULT_WATCHDOG    1.0       // [s]


namespace rd{

/**
 * @ingroup rd_libraries
 *
 * @defgroup ServerLib
 *
 * @brief The ServerLib library contains the Server and RpcResponder classes.
 */

/**
 * @ingroup ServerLib
 *
 * @brief The parent Robot Devastation class of the \ref rdServer program.
 */
class Server : public yarp::os::RFModule
{

    public:
        /**
         * @brief Run the program.
         *
         * @return true/false upon success/failure
         */
        bool configure(yarp::os::ResourceFinder &rf);

        virtual ~Server();

    protected:
        bool interruptModule();
        double getPeriod();
        bool updateModule();

        /** @brief Watchdog period [s]. */
        double watchdog;

        int serverStatus;

        std::map <int, Player> players;
        std::map <int, int> players_belief;
        yarp::os::Mutex players_mutex; //-- To work with players & players_belief

        yarp::os::RpcServer rpcServer;
        RpcResponder rdRpcResponder;
        yarp::os::Port rdBroadcast;
};

} //rd

#endif  // __RD_SERVER_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SERVER_HPP__
#define __RD_SERVER_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Mutex.h>

#include <map>

#include "Utils.hpp"
#include "RpcResponder.hpp"

#define DEFAULT_WATCHDOG    1.0       // [s]


namespace rd{

/**
 * @ingroup rdServer
 * @brief The parent Robot Devastation class of the \ref rdServer program.
 */
class RdServer : public yarp::os::RFModule
{

    public:
        /**
         * Run the program.
         *
         * @return true/false upon success/failure
         */
        bool configure(yarp::os::ResourceFinder &rf);

        virtual ~RdServer();

    protected:
        bool interruptModule();
        double getPeriod();
        bool updateModule();

        /** Watchdog period [s]. */
        double watchdog;

        int serverStatus;

        std::map <int, RdPlayer> players;
        std::map <int, int> players_belief;
        yarp::os::Mutex players_mutex; //-- To work with players & players_belief

        yarp::os::RpcServer rpcServer;
        RdRpcResponder rdRpcResponder;
        yarp::os::Port rdBroadcast;
};

} //rd

#endif  // __RD_SERVER_HPP__


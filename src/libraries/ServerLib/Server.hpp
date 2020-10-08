// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_SERVER_HPP__
#define __RD_SERVER_HPP__

#include <map>
#include <mutex>

#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Port.h>

#include "Player.hpp"
#include "RpcResponder.hpp"

namespace rd{

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

        /**
         * Get the approximate periodicity at which updateModule() is called
         * by runModule()
         *
         * @return period between successive calls to updateModule()
         */
        double getPeriod();

        virtual ~Server();

    protected:
        bool interruptModule();
        bool updateModule();

        /** Disable stdout feedback. */
        bool quiet;

        int serverStatus;

        std::map <int, Player> players;
        std::map <int, int> players_belief;
        std::mutex players_mutex; //-- To work with players & players_belief

        yarp::os::RpcServer rpcServer;
        RpcResponder rdRpcResponder;
        yarp::os::Port rdBroadcast;
};

} //rd

#endif  // __RD_SERVER_HPP__

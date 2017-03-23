// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_NETWORK_MANAGER_HPP__
#define __RD_YARP_NETWORK_MANAGER_HPP__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

#include "Utils.hpp"
#include "NetworkManager.hpp"
#include "NetworkEventListener.hpp"

namespace rd{

/**
 * @ingroup NetworkLib
 *
 * @brief Manages the communications between the user and the Server using YARP
 *
 * YarpNetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the YarpNetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the YarpNetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived)
 *
 */
class YarpNetworkManager: public NetworkManager,
                            private yarp::os::RateThread,
                            public yarp::os::TypedReaderCallback<yarp::os::Bottle>

{
    public:
        //------------------ Creation and configuration --------------------------------------------------------------//
        /**
         * @brief Register this manager in the NetworkManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        ~YarpNetworkManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------ Manager Startup & Halting ---------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped();

        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, Player value);

        //------------------ Server API ----------------------------------------------------------------------------//
        virtual bool sendPlayerHit(Player player, int damage);
        virtual bool login();
        virtual bool logout();
        virtual bool keepAlive();

    protected:
        //! @brief Yarp callback for incoming messages
        void onRead(yarp::os::Bottle& b);

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getNetworkManager() method
         */
        YarpNetworkManager();


        //! @brief Reference to this manager (unique instance)
        static YarpNetworkManager * uniqueInstance;

        void run();

        //! @brief Period of the keep alive (called by run() )
        static const int KEEPALIVE_RATE_MS;


        yarp::os::RpcClient rpcClient;
        yarp::os::BufferedPort<yarp::os::Bottle> callbackPort;

        //! @brief Player
        Player player;

        bool started;
};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

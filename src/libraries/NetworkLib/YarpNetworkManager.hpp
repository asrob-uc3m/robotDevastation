// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_YARP_NETWORK_MANAGER_HPP__
#define __RD_YARP_NETWORK_MANAGER_HPP__

#include <string>

#include <yarp/os/PeriodicThread.h>
#include <yarp/os/PortReaderBuffer.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/BufferedPort.h>

#include "NetworkManager.hpp"
#include "Player.hpp"

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
 * along with the data relevant to the event triggered (i.e. data that just arrived).
 *
 */
class YarpNetworkManager : public NetworkManager,
                           private yarp::os::PeriodicThread,
                           public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
    public:
        //------------------ Creation and configuration --------------------------------------------------------------//
        /**
         * @brief Register this manager in the NetworkManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        virtual ~YarpNetworkManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------ Manager Startup & Halting ---------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;

        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const Player & value);

        //------------------ Server API ----------------------------------------------------------------------------//
        virtual bool sendPlayerHit(const Player & player, int damage);
        virtual bool login();
        virtual bool logout();
        virtual bool keepAlive();

    protected:
        /**
         * @brief Constructor
         *
         * Constructor for this class is non-public since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method. Declared protected to allow
         * subclassing.
         */
        YarpNetworkManager();

        //! @brief Yarp callback for incoming messages
        void onRead(yarp::os::Bottle& b);

        void run();

    private:
        YarpNetworkManager(const YarpNetworkManager &);
        YarpNetworkManager & operator=(const YarpNetworkManager &);

        //! @brief Reference to this manager (unique instance)
        static YarpNetworkManager * uniqueInstance;

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

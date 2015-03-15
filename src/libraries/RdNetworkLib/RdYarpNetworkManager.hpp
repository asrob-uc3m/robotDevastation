// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_NETWORK_MANAGER_HPP__
#define __RD_YARP_NETWORK_MANAGER_HPP__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>

#include "RdUtils.hpp"
#include "RdNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"
#include "RdMentalMapEventListener.hpp"

namespace rd{

/**
 * @ingroup RdNetworkLib
 *
 * @brief Manages the communications between the user and the RdServer using YARP
 *
 * RdYarpNetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdYarpNetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the RdYarpNetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived)
 *
 */
class RdYarpNetworkManager: public RdNetworkManager,
                            public RdMentalMapEventListener,
                            public yarp::os::TypedReaderCallback<yarp::os::Bottle>

{
    public:
        //------------------ Creation and configuration --------------------------------------------------------------//
        /**
         * @brief Register this manager in the RdNetworkManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        ~RdYarpNetworkManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------ Manager Startup & Halting ---------------------------------------------------------------//
        virtual bool start();
        virtual bool stop();

        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);

        //------------------ RdServer API ----------------------------------------------------------------------------//
        virtual bool sendPlayerHit(RdPlayer player, int damage);
        virtual bool login(RdPlayer player);
        virtual bool logout(RdPlayer player);

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
        RdYarpNetworkManager();


        //! @brief Reference to this manager (unique instance)
        static RdYarpNetworkManager * uniqueInstance;


        /**
         * @brief Start the NetworkManager communications with the RdServer
         * @param id Id of the player corresponding to the user
         * @return
         */


        yarp::os::RpcClient rpcClient;
        yarp::os::BufferedPort<yarp::os::Bottle> callbackPort;

        //! @brief Implementation of RdMentalMapEventListener interface
        bool onTargetHit(RdTarget target, RdPlayer player, RdWeapon weapon);

        //! @brief Identifier for the player
        int player_id;
};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

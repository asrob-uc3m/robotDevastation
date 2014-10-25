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
        //-- Creation and configuration
        //--------------------------------------------------------------------------------------------
        //! @brief Get a reference to RdNetworkManager
        static RdNetworkManager * getNetworkManager();

        //! @brief Deallocate the RdNetworkManager
        static bool destroyNetworkManager();

        ~RdYarpNetworkManager();

        //-- RdServer API
        //--------------------------------------------------------------------------------------------
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
         * and the instantiation is done at the getMentalMap() method
         */
        RdYarpNetworkManager();

        /**
         * @brief Start the NetworkManager communications with the RdServer
         * @param id Id of the player corresponding to the user
         * @return
         */
        bool start(int id);


        yarp::os::RpcClient rpcClient;
        yarp::os::BufferedPort<yarp::os::Bottle> callbackPort;

        //! @brief Implementation of RdMentalMapEventListener interface
        bool onTargetHit(RdTarget target, RdPlayer player, RdWeapon weapon);
};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

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

class RdYarpNetworkManager: public RdNetworkManager,
                            public RdMentalMapEventListener,
                            public yarp::os::TypedReaderCallback<yarp::os::Bottle>

{
    public:
        //-- Singleton functions to get an instance / remove it
        static RdNetworkManager * getNetworkManager();
        static bool destroyNetworkManager();

        ~RdYarpNetworkManager();

        //-- User interface to access to the server
        virtual bool sendPlayerHit(RdPlayer player, int damage);
        virtual bool login(RdPlayer player);
        virtual bool logout(RdPlayer player);

    protected:
        //-- Yarp event for incoming messages
        void onRead(yarp::os::Bottle& b);

    private:
        bool start(int id);
        RdYarpNetworkManager();

        yarp::os::RpcClient rpcClient;
        yarp::os::BufferedPort<yarp::os::Bottle> callbackPort;

        //-- Implementation of RdMentalMapEventListener interface
        bool onTargetHit(RdTarget target, RdPlayer player, RdWeapon weapon);
};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

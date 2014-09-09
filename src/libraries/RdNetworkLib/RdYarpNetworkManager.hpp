// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_NETWORK_MANAGER_HPP__
#define __RD_YARP_NETWORK_MANAGER_HPP__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>

#include "RdUtils.hpp"
#include "RdNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"

namespace rd{

class RdYarpNetworkManager: public RdNetworkManager,
        public yarp::os::BufferedPort<yarp::os::Bottle>

{
    public:
        static RdNetworkManager * getNetworkManager();
        static bool destroyNetworkManager();

        ~RdYarpNetworkManager();


    protected:
        virtual bool sendPlayer(RdPlayer player);
        void onRead(yarp::os::Bottle& b);

    private:
        RdYarpNetworkManager();

        static RdNetworkManager * networkManagerInstance;


};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__

// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_RPC_RESPONDER__
#define __RD_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Time.h>
#include <yarp/os/Vocab.h>
#include <yarp/os/Mutex.h>

#include <map>

#include "RdUtils.hpp"
#include "RdPlayer.hpp"

namespace rd
{

class RdRpcResponder : public yarp::os::PortReader
{
public:
    void setPlayers(std::map<int,RdPlayer> *value, std::map <int,int>* players_belief,
                    yarp::os::Mutex* players_mutex);

    static const int MAX_BELIEF; //-- Timeout [s] / Period [s]

private:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(yarp::os::ConnectionReader& connection);

    yarp::os::Port* rdBroadcast;
    std::map <int,RdPlayer>* players;
    std::map <int,int>* players_belief;
    yarp::os::Mutex* players_mutex;
};

}

#endif


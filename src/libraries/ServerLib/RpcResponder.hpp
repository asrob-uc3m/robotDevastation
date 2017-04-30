// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_RPC_RESPONDER__
#define __RD_RPC_RESPONDER__

#include <map>

#include <yarp/os/PortReader.h>
#include <yarp/os/Mutex.h>
#include <yarp/os/ConnectionReader.h>
#include <yarp/os/Port.h>

#include "Player.hpp"

namespace rd
{

/**
 * @ingroup ServerLib
 *
 * @brief RPC broadcast port
 */
class RpcResponder : public yarp::os::PortReader
{
public:
    void setPlayers(std::map<int,Player> *value, std::map <int,int>* players_belief,
                    yarp::os::Mutex* players_mutex);

    //! @brief Timeout [s] / Period [s]
    static const int MAX_BELIEF;

private:
    //! @brief Implements the actual responder (callback on RPC).
    virtual bool read(yarp::os::ConnectionReader& connection);

    yarp::os::Port* rdBroadcast;
    std::map <int,Player>* players;
    std::map <int,int>* players_belief;
    yarp::os::Mutex* players_mutex;
};

}

#endif


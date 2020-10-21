// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_RPC_RESPONDER__
#define __RD_RPC_RESPONDER__

#include <map>
#include <mutex>

#include <yarp/os/PortReader.h>
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
                    std::mutex* players_mutex);

    //! @brief Timeout [s] / Period [s]
    static const int MAX_BELIEF;

private:
    //! @brief Implements the actual responder (callback on RPC).
    virtual bool read(yarp::os::ConnectionReader& connection);

    yarp::os::Port* rdBroadcast;
    std::map <int,Player>* players;
    std::map <int,int>* players_belief;
    std::mutex* players_mutex;
};

}

#endif

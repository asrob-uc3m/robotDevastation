// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_SOCKET_HPP__
#define __RD_NETWORK_SOCKET_HPP__

#include "RdNetworkBase.hpp"

#include <sys/types.h>
#include <sys/socket.h>  // inet_addr in linux.
#include <netinet/in.h>  // sockaddr_in in linux.
#include <arpa/inet.h>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 5000

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdNetworkLib
 *
 * @brief The RdNetworkLib library contains the \ref RdNetworkBase class and some inherited implementations.
 */

namespace rdlib{

/**
 * @ingroup RdNetworkLib
 *
 * @brief A class for Robot Devastation that uses native sockets.
 *
 */
class RdNetworkSocket : public RdNetworkBase {
    public:

        virtual bool setup();
        virtual bool pointerIncrement();



    protected:
        struct sockaddr_in destAddr;
        int testSocket;
};

} //rdlib

#endif  // __RD_NETWORK_SOCKET_HPP__



// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_SOCKET_HPP__
#define __RD_NETWORK_SOCKET_HPP__

#include "RdNetworkBase.hpp"

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


    protected:


};

} //rdlib

#endif  // __RD_NETWORK_SOCKET_HPP__



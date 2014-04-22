// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_NETWORK_BASE_HPP__
#define __RD_NETWORK_BASE_HPP__

#include <iostream>
#include <vector>
#include <stdlib.h>  // getenv

#include "RdMacros.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdNetworkLib
 *
 * @brief The RdNetworkLib library contains the \ref RdNetworkBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;

/**
 * @ingroup RdNetworkLib
 *
 * @brief An abstract base class for Robot Devastation image processors.
 */
class RdNetworkBase {
    public:
        virtual bool setup()=0;
        virtual bool pointerIncrement()=0;

    protected:


};

} //rdlib

#endif  // __RD_NETWORK_BASE_HPP__



// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_PROCESS_BASE_HPP__
#define __RD_IMAGE_PROCESS_BASE_HPP__

#include <iostream>
#include <vector>

#include "RdMacros.hpp"
#include "RdManagerBase.hpp"
#include "RdEnemy.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdImageProcessLib
 *
 * @brief The RdImageProcessLib library contains the \ref RdImageProcessBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;

/**
 * @ingroup RdImageProcessLib
 *
 * @brief An abstract base class for Robot Devastation image processors.
 */
class RdImageProcessBase {
    public:
        virtual bool setup()=0;
        virtual bool process( char * imgPtr, const int width, const int height, const int step,
                              std::vector< RdEnemy* >& enemies )=0;

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr )
        {
            this->rdManagerBasePtr = rdManagerBasePtr;
        }

    protected:

        //-- Pointers to other modules
        RdManagerBase* rdManagerBasePtr;

};

} //rdlib

#endif  // __RD_IMAGE_PROCESS_BASE_HPP__



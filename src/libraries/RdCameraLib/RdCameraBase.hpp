// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_CAMERA_BASE_HPP__
#define __RD_CAMERA_BASE_HPP__

#include <iostream>
#include "RdManagerBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdCameraLib
 *
 * @brief The RdCameraLib library contains the \ref RdCameraBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;
class RdRobotBase;

/**
 * @ingroup RdCameraBase
 *
 * @brief An abstract base class for Robot Devastation camera inputs (webcams, stereoscopic cams, etc).
 *
 */
class RdCameraBase {
    public:
        virtual bool quit() =0;

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr ) {
            this->rdManagerBasePtr = rdManagerBasePtr;
        }

    protected:
        RdManagerBase* rdManagerBasePtr;
};

} //rdlib

#endif  // __RD_CAMERA_BASE_HPP__

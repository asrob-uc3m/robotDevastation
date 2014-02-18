// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_BASE_HPP__
#define __RD_INPUT_BASE_HPP__

#include <iostream>
#include "RdRobotBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library contains the \ref RdInputBase class and some inherited implementations.
 */

namespace rdlib{

/**
 * @ingroup RdInputLib
 *
 * @brief An abstract base class for Robot Devastation inputs (keyboard, xpad...).
 *
 */
class RdInputBase {
    public:
        /** An init rutine.
         * @return true if the object was init successfully.
         */
        virtual bool stop() = 0;

        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr ) {
            this->rdRobotBasePtr = rdRobotBasePtr;
        }

    protected:
        RdRobotBase* rdRobotBasePtr;
};

} //rdlib

#endif  // __RD_INPUT_BASE_HPP__


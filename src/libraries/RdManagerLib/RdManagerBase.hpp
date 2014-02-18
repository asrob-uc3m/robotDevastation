// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_BASE_HPP__
#define __RD_MANAGER_BASE_HPP__

#include <iostream>
#include "RdInputBase.hpp"
#include "RdRobotBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdManagerLib
 *
 * @brief The RdManagerLib library contains the \ref RdManagerBase class and some inherited implementations.
 */

namespace rdlib{

class RdInputBase;
class RdRobotBase;

/**
 * @ingroup RdManagerLib
 *
 * @brief An abstract base class for Robot Devastation inputs (keyboard, xpad...).
 *
 */
class RdManagerBase {
    public:
        /** An shoot rutine.
         * @return true if successful.
         */
        virtual bool shoot() = 0;

        void setRdInputBasePtr(RdInputBase* rdInputBasePtr ) {
            this->rdInputBasePtr = rdInputBasePtr;
        }
        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr ) {
            this->rdRobotBasePtr = rdRobotBasePtr;
        }

    protected:
        RdInputBase* rdInputBasePtr;
        RdRobotBase* rdRobotBasePtr;
};

} //rdlib

#endif  // __RD_MANAGER_BASE_HPP__


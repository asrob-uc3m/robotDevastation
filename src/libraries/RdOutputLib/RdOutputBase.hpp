// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_OUTPUT_BASE_HPP__
#define __RD_OUTPUT_BASE_HPP__

#include <iostream>
#include "RdManagerBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library contains the \ref RdInputBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;
class RdRobotBase;

/**
 * @ingroup RdOuputLib
 *
 * @brief An abstract base class for Robot Devastation ouputs (screen...).
 *
 */
class RdOuputBase {
    public:
        /** An init rutine.
         * @return true if the object was init successfully.
         */
        virtual bool stop() = 0;

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr ) {
            this->rdManagerBasePtr = rdManagerBasePtr;
        }

    protected:
        RdManagerBase* rdManagerBasePtr;
};

} //rdlib

#endif  // __RD_OUTPUT_BASE_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_BASE_HPP__
#define __RD_INPUT_BASE_HPP__

#include <iostream>

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
        /** An initialization rutine.
         * @return true if the object was initialized successfully.
         */
        virtual bool init() = 0;

        /** A closing rutine.
         * @return true if the object was closed successfully
         */
        virtual bool stop() = 0;

    protected:
        bool quiet;
};

} //rdlib

#endif  // __RD_INPUT_BASE_HPP__


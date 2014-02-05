// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_BASE_HPP__
#define __RD_INPUT_BASE_HPP__

#include <iostream>

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library is a collection of a simple lib for inputs (keyboard, xpad...).
 */

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief A base class for inputs (keyboard, xpad...).
 *
 */
class RdInputBase {
    public:
        virtual bool init() = 0;
        virtual bool stop() = 0;
    protected:
        bool quiet;
};

} //rd

#endif  // __RD_INPUT_BASE_HPP__


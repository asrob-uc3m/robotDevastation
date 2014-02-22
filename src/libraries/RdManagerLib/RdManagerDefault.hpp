// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_DEFAULT_HPP__
#define __RD_MANAGER_DEFAULT_HPP__

#include "RdManagerBase.hpp"

namespace rdlib{

/**
 * @ingroup RdManagerLib
 *
 * @brief Implements a default manager for Robot Devastation.
 */
class RdManagerDefault : public RdManagerBase {
    public:
        RdManagerDefault();

        virtual bool quit();

        virtual bool shoot();

    protected:
};

} //rdlib

#endif  // __RD_MANAGER_DEFAULT_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_DEFAULT_HPP__
#define __RD_MANAGER_DEFAULT_HPP__

#include "RdManagerBase.hpp"
#include <semaphore.h>
#include <vector>
#include <utility>
#include <pthread.h>

#include "RdImageProcessOpenCV.hpp"

namespace rdlib{

/**
 * @ingroup RdManagerLib
 *
 * @brief Implements a default manager for Robot Devastation.
 */
class RdManagerDefault : public RdManagerBase {
    public:
        RdManagerDefault();

        virtual bool callFunctionByName(const std::string &cmd);
        virtual bool shoot();
        virtual bool setup();

        virtual bool manage(int pipelineIndex);

        bool toggleHeadTrack();
   protected:
        static const int MANAGER_STATUS_HEAD_TRACK = 1;
        int width, height, step;
};

} //rdlib

#endif  // __RD_MANAGER_DEFAULT_HPP__


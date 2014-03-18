// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_DEFAULT_HPP__
#define __RD_MANAGER_DEFAULT_HPP__

#include "RdManagerBase.hpp"
#include <semaphore.h>
#include <vector>
#include <utility>
#include <pthread.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace rdlib{

/**
 * @ingroup RdManagerLib
 *
 * @brief Implements a default manager for Robot Devastation.
 */
class RdManagerDefault : public RdManagerBase {
    public:
        RdManagerDefault();

        virtual bool shoot();

        virtual bool processImage();

        static bool toggleHeadTrackWrapper( void * This);
        bool toggleHeadTrack();
        bool trackHead( int index); //-- Index is the source image that is being processed
   protected:
        static const int MANAGER_STATUS_HEAD_TRACK = 1;

};

} //rdlib

#endif  // __RD_MANAGER_DEFAULT_HPP__


// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_PROCESS_ZBAR_HPP__
#define __RD_IMAGE_PROCESS_ZBAR_HPP__

#include "RdImageProcessBase.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstddef>
#include "zbar.h"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdImageProcessLib
 *
 * @brief The RdImageProcessLib library contains the \ref RdImageProcessBase class and some inherited implementations.
 */

namespace rdlib{

/**
 * @ingroup RdImageProcessLib
 *
 * @brief A class for Robot Devastation that uses Zbar to process images.
 *
 */
class RdImageProcessZbar : public RdImageProcessBase {
    public:

        virtual bool setup();
        virtual bool process(char * ptr , const int width, const int height, const int step,
                             std::vector< RdEnemy >* vectorOfRdEnemy, pthread_mutex_t* mutexOfVectorOfRdEnemy );

        bool trackHead( int index );

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        //virtual bool quit();

    protected:
        zbar::ImageScanner scanner;
        std::vector<RdEnemy> localVectorOfRdEnemy;

};

} //rdlib

#endif  // __RD_IMAGE_PROCESS_ZBAR_HPP__



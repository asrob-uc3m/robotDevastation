// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_PROCESS_OPENCV_HPP__
#define __RD_IMAGE_PROCESS_OPENCV_HPP__

#include "RdImageProcessBase.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
 * @brief A class for Robot Devastation that uses OpenCV to process images.
 *
 */
class RdImageProcessOpenCV : public RdImageProcessBase {
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
        /** Create a haars cascade classifier */
        cv::CascadeClassifier faceDetector;

        std::vector< std::pair<int, int> > enemiesFound;
        std::vector< double > enemiesFoundSize;

};

} //rdlib

#endif  // __RD_IMAGE_PROCESS_OPENCV_HPP__



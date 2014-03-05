// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_CAMERA_WEBCAM_HPP__
#define __RD_CAMERA_WEBCAM_HPP__

#include <iostream>
#include "RdCameraBase.hpp"


#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // usleep
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/highgui/highgui.hpp"

namespace rdlib{

/**
 * @ingroup RdCameraWebcam
 *
 * @brief An class for Robot Devastation webcam input using openCV.
 *
 */
class RdCameraWebcam : public RdCameraBase
{
    public:
        RdCameraWebcam( int cameraIndex = 1 );

        virtual void capture(int index);

        virtual char * getBufferPtr( int index = 0);
        virtual bool getDimensions( int& width, int& height, int &step);

    protected:
        cv::VideoCapture webcam;
        std::vector<cv::Mat> imageBuffers;
        int frameRate; //-- Capture period (ms)

};

} //rdlib

#endif  // __RD_CAMERA_WEBCAM_HPP__

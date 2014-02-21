// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_CAMERA_WEBCAM_HPP__
#define __RD_CAMERA_WEBCAM_HPP__

#include <iostream>
#include "RdCameraBase.hpp"


#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/highgui/highgui.hpp"

namespace rdlib{

/**
 * @ingroup RdCameraWebcam
 *
 * @brief An class for Robot Devastation webcam input using openCV.
 *
 */
class RdCameraWebcam {
    public:
        RdCameraWebcam( int index = -1 );

        /** An init rutine.
         * @return true if the object was init successfully.
         */
        virtual bool stop() = 0;

        virtual bool quit();

        static void * captureThread( void * This );

        void capture();

        bool setStop( bool stop);
        char * getBufferPtr();
        bool getDimensions( int& width, int& height, int &step);

    protected:
        pthread_t capture_thread;
        pthread_mutex_t imageBufferMutex;
        cv::VideoCapture webcam;
        cv::Mat imageBuffer;
        int frameRate; //-- Capture period (ms)
        bool stop;
};

} //rdlib

#endif  // __RD_CAMERA_WEBCAM_HPP__

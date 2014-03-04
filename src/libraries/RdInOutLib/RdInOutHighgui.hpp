// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IN_OUT_HIGHGUI_HPP__
#define __RD_IN_OUT_HIGHGUI_HPP__

#include "RdOutputBase.hpp"
#include "RdCameraBase.hpp"
#include "RdIniReader.hpp"

#undef index  // avoid compilation "index" macro arguments error with opencv
#include <opencv2/highgui/highgui.hpp>
#include <semaphore.h>

namespace rdlib{

/**
 * @ingroup RdInOutLib
 *
 * @brief Implements an OpenCV libhighgui input/output for Robot Devastation.
 */
class RdInOutHighgui : public RdOutputBase, public RdInputBase {
    public:
        RdInOutHighgui();

        virtual bool start();
        virtual bool quit();

        static void * highguiThread( void * This );

        void output();
        bool printEnemies(cv::Mat& src, cv::Mat& dst, int index);

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr );

        void setDisplaySemaphores( sem_t * displaySemaphores);
        void setCaptureSemaphores( sem_t * captureSemaphores);

    protected:
        sem_t * captureSemaphores;
        sem_t * displaySemaphores;
        pthread_t highgui_thread;

        std::vector<cv::Mat> videoFrames;
        rdlib::RdCameraBase* rdCameraBasePtr;

        RdManagerBase* rdManagerBasePtr;  //--solve ambiguity

        bool isRunning;

};

} //rdlib

#endif  // __RD_IN_OUT_HIGHGUI_HPP__


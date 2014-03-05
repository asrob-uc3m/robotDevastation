// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_OUTPUT_HIGHGUI_HPP__
#define __RD_OUTPUT_HIGHGUI_HPP__

#include "RdOutputBase.hpp"
#include "RdCameraBase.hpp"

#undef index  // avoid compilation "index" macro arguments error with opencv
#include <opencv2/highgui/highgui.hpp>

namespace rdlib{

/**
 * @ingroup RdOutputLib
 *
 * @brief Implements an OpenCV libhighgui output for Robot Devastation.
 */
class RdOutputHighgui : public RdOutputBase {
    public:
        RdOutputHighgui();

        char * getInputKey();
        pthread_mutex_t * getInputKeyMutex();

    protected:
        virtual bool output(int pipelineIndex);

        cv::Mat videoFrame;

        //-- References to other modules
        RdCameraBase* rdCameraBasePtr;

        //-- Add queue here (and its mutex)
        char * inputKey;
        pthread_mutex_t * inputKeyMutex;
};

} //rdlib

#endif  // __RD_OUTPUT_HIGHGUI_HPP__


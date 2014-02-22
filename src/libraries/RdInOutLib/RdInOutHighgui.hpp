// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IN_OUT_HIGHGUI_HPP__
#define __RD_IN_OUT_HIGHGUI_HPP__

#include "RdOutputBase.hpp"
#include "RdCameraBase.hpp"

#undef index  // avoid compilation "index" macro arguments error with opencv
#include <opencv2/highgui/highgui.hpp>

namespace rdlib{

/**
 * @ingroup RdInOutLib
 *
 * @brief Implements an OpenCV libhighgui input/output for Robot Devastation.
 */
class RdInOutHighgui : public RdOutputBase, public RdInputBase {
    public:
        RdInOutHighgui();

        virtual bool quit();

        static void * outputThread( void * This );

        void output();

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr ) {  //--solve ambiguity
            this->rdManagerBasePtr = rdManagerBasePtr;
        }

    protected:
        pthread_t output_thread;

        cv::Mat videoFrame;
        rdlib::RdCameraBase* rdCameraBasePtr;

        RdManagerBase* rdManagerBasePtr;  //--solve ambiguity

};

} //rdlib

#endif  // __RD_IN_OUT_HIGHGUI_HPP__


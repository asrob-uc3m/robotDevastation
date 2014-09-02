// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RATE_THREAD_PROCESS_HPP__
#define __RATE_THREAD_PROCESS_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/RateThread.h>

#include <yarp/sig/Image.h>

// tmp use of opencv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <zbar.h>

#include "RdMacros.hpp"
#include "RdMentalMap.hpp"

#define DEFAULT_RATE_MS 20

namespace rd
{

class RateThreadProcess : public yarp::os::RateThread {
    private:
        yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > *pInImg;
        //
        int rateMs;
        //
        int cameraDepth;  // the depth of the surface in bits
        int cameraChannels;  // R G B
        int cameraWidthstep; // the length of a row of pixels in bytes 1920 for 640x480
        int cameraWidth;
        int cameraHeight;
        bool cameraInitialized;
        //
        zbar::ImageScanner scanner;
        //
        RdPlayer* myPlayer;

        RdMentalMap* mentalMap;

    public:
        RateThreadProcess() : RateThread(DEFAULT_RATE_MS) {}

        void setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * _pInImg);

        void init(yarp::os::ResourceFinder &rf);
        void run();  // The periodical function

        void setMyPlayer(RdPlayer *value);

        void setMentalMap(RdMentalMap *value);


};

}  // namespace rd

#endif  // __RATE_THREAD_PROCESS_HPP__


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

#include <math.h>  //-- Just for fabs to make it a bit failproof.

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
        int cameraWidth;
        int cameraHeight;
        bool cameraInitialized;
        //
        zbar::ImageScanner scanner;

        RdMentalMap* mentalMap;

    public:
        RateThreadProcess() : RateThread(DEFAULT_RATE_MS) { mentalMap = RdMentalMap::getMentalMap(); }

        void setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * _pInImg);

        void init(yarp::os::ResourceFinder &rf);
        void run();  // The periodical function
};

}  // namespace rd

#endif  // __RATE_THREAD_PROCESS_HPP__


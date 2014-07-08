// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __EVENT_INPUT_HPP__
#define __EVENT_INPUT_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Thread.h>

#include <yarp/sig/all.h>

#include <SDL/SDL_image.h>

#include "RdMacros.hpp"

#define DEFAULT_RATE_MS 20

namespace rd
{

class EventInput : public yarp::os::Thread {
    private:
        //yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > *pInImg;
        //BufferedPort<ImageOf<PixelRgb> > *pOutImg;  // for testing
        //Port *pOutPort;
        //
        //int rateMs;
        //
        //SDL_Surface* display;
        //
        //int cameraDepth;  // the depth of the surface in bits
        //int cameraChannels;  // R G B
        //int cameraWidthstep; // the length of a row of pixels in bytes 1920 for 640x480
        //int cameraWidth;
        //int cameraHeight;
        //bool cameraInitialized;

    public:
        //EventInput() : RateThread(DEFAULT_RATE_MS) {}

        //void setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * _pInImg);
        //void setOutImg(BufferedPort<ImageOf<PixelRgb> > * _pOutImg);
        //void setOutPort(Port *_pOutPort);
        //void init(yarp::os::ResourceFinder &rf);
        void run();  // The function
};

}  // namespace rd

#endif  // __EVENT_INPUT_HPP__


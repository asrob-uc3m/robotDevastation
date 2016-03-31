// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RATE_THREAD_OUTPUT_HPP__
#define __RATE_THREAD_OUTPUT_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/RateThread.h>

#include <yarp/sig/all.h>

#include <vector>

#include <SDL/SDL_image.h>

#include "RdMacros.hpp"
#include "RdGameScreen.hpp"
#include "RdImageManager.hpp"
#include "SDLUtils.hpp"

#define DEFAULT_RATE_MS 20

namespace rd
{

class RateThreadOutput : public yarp::os::RateThread {
    private:

        int rateMs;
        //
        SDL_Surface* display;
        //
        int cameraDepth;  // the depth of the surface in bits
        int cameraChannels;  // R G B
        int cameraWidthstep; // the length of a row of pixels in bytes 1920 for 640x480
        int cameraWidth;
        int cameraHeight;
        bool cameraInitialized;

        RdGameScreen gameScreen;

        RdImageManager * imageManager;

    public:
        RateThreadOutput() : RateThread(DEFAULT_RATE_MS) {}

        void init(yarp::os::ResourceFinder &rf);
        void run();  // The periodical function
};

}  // namespace rd

#endif  // __RATE_THREAD_OUTPUT_HPP__


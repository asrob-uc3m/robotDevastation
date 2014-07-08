// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>

#include "RateThreadOutput.hpp"
#include "RdAudioManager.hpp"

namespace rd
{

class RobotDevastation : public yarp::os::RFModule
{
    private:
        RateThreadOutput rateThreadOutput;
        //
        yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > inImg;
        //BufferedPort<ImageOf<PixelRgb> > outImg;
        //Port outPort;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initSound();

    public:
        bool configure(yarp::os::ResourceFinder &rf);
        RdAudioManager audioManager;

};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__


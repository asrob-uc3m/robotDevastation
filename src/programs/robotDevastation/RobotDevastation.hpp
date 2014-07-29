// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/RpcClient.h>

#include "RateThreadOutput.hpp"
#include "RateThreadProcess.hpp"
#include "EventInput.hpp"
#include "RdAudioManager.hpp"
#include "RdPlayer.hpp"
#include "CallbackPort.hpp"

#define VOCAB_RD_LOGIN VOCAB3('l','o','g')

namespace rd
{

class RobotDevastation : public yarp::os::RFModule
{
    public:
        bool configure(yarp::os::ResourceFinder &rf);

    private:
        RateThreadOutput rateThreadOutput;
        RateThreadProcess rateThreadProcess;
        EventInput eventInput;
        RdAudioManager audioManager;
        //
        yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > inImg;
        yarp::os::RpcClient rpcClient;
        CallbackPort callbackPort;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initSound();

};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__


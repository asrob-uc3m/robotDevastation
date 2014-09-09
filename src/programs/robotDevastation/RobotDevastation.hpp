// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/Semaphore.h>

#include "RdUtils.hpp"
#include "RateThreadOutput.hpp"
#include "RateThreadProcess.hpp"
#include "EventInput.hpp"
#include "CallbackPort.hpp"
#include "RdAudioManager.hpp"
#include "RdMentalMap.hpp"
#include "RdInputManager.hpp"
#include "RdInputEventListener.hpp"

namespace rd
{

class RobotDevastation : public yarp::os::RFModule, public RdInputEventListener
{
    public:
        bool configure(yarp::os::ResourceFinder &rf);

        virtual bool onKeyPressed(RdKey k);

    private:
        RateThreadOutput rateThreadOutput;
        RateThreadProcess rateThreadProcess;
        RdInputManager *  inputManager;
        RdAudioManager * audioManager;
        //
        yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > inImg;
        yarp::os::RpcClient rpcClient;
        CallbackPort callbackPort;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initSound();

        RdMentalMap * mentalMap;

        char* rdRoot;

};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__


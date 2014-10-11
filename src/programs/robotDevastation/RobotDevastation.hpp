// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>

#include "RdUtils.hpp"
#include "RateThreadOutput.hpp"
#include "RateThreadProcess.hpp"
#include "RdAudioManager.hpp"
#include "RdMentalMap.hpp"
#include "RdInputManager.hpp"
#include "RdInputEventListener.hpp"
#include "RdYarpNetworkManager.hpp"
#include "RdRobotManager.hpp"
#include "RdRd1RobotManager.hpp"
#include "RdStateMachine.hpp"

namespace rd
{

class RobotDevastation : public yarp::os::RFModule, public RdInputEventListener
{
    public:
        bool configure(yarp::os::ResourceFinder &rf);

        virtual bool onKeyUp(RdKey k);
        virtual bool onKeyDown(RdKey k);

    private:
        RateThreadOutput rateThreadOutput;
        RateThreadProcess rateThreadProcess;
        RdInputManager *  inputManager;
        RdAudioManager * audioManager;
        RdMentalMap * mentalMap;
        RdNetworkManager * networkManager;
        RdRobotManager * robotManager;
        RdStateMachine * stateMachine;

        yarp::os::BufferedPort< yarp::sig::ImageOf < yarp::sig::PixelRgb> > inImg;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initSound();

        char* rdRoot;

};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__

